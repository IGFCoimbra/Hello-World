package com.crowdstreet.investorexperience.webhead.resource.authenticated;

import static com.crowdstreet.connect.helper.EmailHelper.deriveRecipientBaseUrl;

import com.crowdstreet.common.LoggingProvider;
import com.crowdstreet.common.PostgresConnectionProvider;
import com.crowdstreet.common.model.User;
import com.crowdstreet.common.template.TemplateProcessor;
import com.crowdstreet.connect.entities.InvestorEmailEntity;
import com.crowdstreet.connect.entities.InvestorEmailMetadataEntity;
import com.crowdstreet.connect.entities.InvestorEmailWithAttachmentsEntity;
import com.crowdstreet.connect.entities.keys.impl.EmailID;
import com.crowdstreet.connect.entities.keys.impl.PortalId;
import com.crowdstreet.connect.entities.keys.impl.UserProfileID;
import com.crowdstreet.connect.helper.PortalHelper;
import com.crowdstreet.connect.service.InvestorEmailService;
import com.crowdstreet.investorexperience.webhead.model.MessageContentModel;
import com.crowdstreet.investorexperience.webhead.model.MessageMetadata;
import com.crowdstreet.managementportal.model.DocumentLink;
import com.google.common.base.Strings;
import com.google.common.collect.ImmutableMap;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.*;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;
import javax.inject.Inject;
import javax.inject.Singleton;
import javax.ws.rs.ForbiddenException;
import javax.ws.rs.GET;
import javax.ws.rs.InternalServerErrorException;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import org.slf4j.Logger;

@Singleton
@Path("/message-center")
public class MessageCenterResource {
    private static final Logger log =
            LoggingProvider.INSTANCE.getLogger(MessageCenterResource.class);

    private final InvestorEmailService investorEmailService;

    @Inject
    public MessageCenterResource(final InvestorEmailService investorEmailService) {
        this.investorEmailService = investorEmailService;
    }

    @GET
    @Path("/user/{userProfileId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMessageMetaData(
            @Context User user,
            @PathParam("userProfileId") UserProfileID userProfileID,
            @QueryParam("preview") String previewUuid) {

        validateAcesss(user, previewUuid);

        final List<InvestorEmailEntity> emails =
                investorEmailService.getByUserProfileId(userProfileID);
        final List<InvestorEmailMetadataEntity> emailMetadata =
                investorEmailService.getEmailMetadata(
                        emails.stream()
                                .map(InvestorEmailEntity::getOfferId)
                                .filter(Optional::isPresent)
                                .map(Optional::get)
                                .collect(Collectors.toSet()));

        final List<MessageMetadata> metadata =
                emails.stream()
                        .map(email -> MessageMetadata.fromInvestorEmail(email, emailMetadata))
                        .collect(Collectors.toList());

        return Response.ok(ImmutableMap.of("metadata", metadata)).build();
    }

    /*
     * Called by contact detail v1 page
     */
    @GET
    @Path("/get-message-content")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMessageContent(
            @Context PortalId portalId,
            @Context User user,
            @QueryParam("messageId") String messageId,
            @QueryParam("preview") String previewUuid) {
        return internalGetMesageContent(user, portalId, new EmailID(messageId), previewUuid);
    }

    @GET
    @Path("/email/{emailId}/content")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMessageContent(
            @Context PortalId portalId,
            @Context User user,
            @PathParam("emailId") EmailID emailId,
            @QueryParam("preview") String previewUuid) {
        return internalGetMesageContent(user, portalId, emailId, previewUuid);
    }

    private Response internalGetMesageContent(
            User user, PortalId portalId, EmailID emailId, String previewUuid) {
        validateAcesss(user, previewUuid);

        final InvestorEmailWithAttachmentsEntity investorEmailEntity =
                investorEmailService.getFull(portalId, emailId).orElseThrow(NotFoundException::new);

        final String baseUrl;
        try {
            baseUrl =
                    deriveRecipientBaseUrl(
                            ImmutableMap.<String, String>builder()
                                    .put("private_portal_id", user.getPortalId().toString())
                                    .build(),
                            PortalHelper.getBaseUrls());
        } catch (SQLException e) {
            throw new InternalServerErrorException(e);
        }

        final Set<DocumentLink> documentLinks =
                investorEmailEntity.getAttachments().stream()
                        .map(DocumentLink::fromInvestorEmailAttachment)
                        .collect(Collectors.toSet());

        final MessageContentModel model =
                new MessageContentModel(investorEmailEntity.getBody(), baseUrl, documentLinks);

        return Response.ok(model).build();
    }

    public static User getUser(String uuid) {
        try {
            String query =
                    TemplateProcessor.ftlToString(
                            "/queries/investorexperience/portfolio/get_user_by_uuid.sql",
                            Collections.emptyMap());
            try (final Connection co =
                            PostgresConnectionProvider.INSTANCE.getConnection(
                                    PostgresConnectionProvider.CROWDSTREET_DB_ID);
                    final PreparedStatement ps = co.prepareStatement(query); ) {

                ps.setObject(1, PostgresConnectionProvider.getUuidObject(uuid));
                try (final ResultSet rs = ps.executeQuery()) {
                    if (rs.next()) {
                        User u = new User();
                        u.setFirstName(rs.getString(1));
                        u.setLastName(rs.getString(2));
                        u.setUuid(rs.getString(3));
                        u.setPortalName(rs.getString(4));
                        u.setUserId(rs.getLong(5));
                        u.setUserProfileId(rs.getLong(6));
                        u.setSponsorId(rs.getInt(7));
                        u.setPortalId(rs.getInt(8));
                        if (rs.getBoolean(9)) {
                            u.getRoles().add(User.Role.staff);
                        }
                        if (rs.getBoolean(10)) {
                            u.getRoles().add(User.Role.superuser);
                        }
                        if (rs.getBoolean(11)) {
                            u.getRoles().add(User.Role.employee);
                        }
                        u.setEmail(rs.getString(12));
                        u.setSponsorTitle(rs.getString(13));
                        return u;
                    }
                }
            }
        } catch (Exception ex) {
            LoggingProvider.INSTANCE
                    .getLogger(MessageCenterResource.class)
                    .error("Error getting user by uuid", ex);
        }
        return null;
    }

    private void validateAcesss(final User loggedInUser, final String previewUuid) {
        if (Strings.isNullOrEmpty(previewUuid)) {
            return;
        }
        User previewUser = getUser(previewUuid);
        if (previewUser != null) {
            if (loggedInUser.getNormalizedRole() == User.Role.superuser) {
                return;
            }

            if (loggedInUser.getNormalizedRole() == User.Role.staff
                    && previewUser.getPortalId() == 0) {
                return;
            }

            if (loggedInUser.getNormalizedRole() == User.Role.employee
                    && previewUser.getPortalId().intValue()
                            == loggedInUser.getPortalId().intValue()) {
                return;
            }
        }

        throw new ForbiddenException();
    }
}
