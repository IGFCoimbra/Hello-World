/** Licensed under AMC Networks Inc. */
package com.amcn.api.controller.impl;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.atMostOnce;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;

import com.amcn.api.exception.AppExceptionHandler;
import com.amcn.api.exception.BadRequestException;
import com.amcn.api.exception.TokenNotFoundException;
import com.amcn.api.model.rest.SchemasGenerateToken;
import com.amcn.api.model.rest.SchemasGeneratedToken;
import com.amcn.api.model.rest.SchemasTokenData;
import com.amcn.api.model.rest.SchemasTokenDataData;
import com.amcn.api.model.rest.SchemasTokenType;
import com.amcn.api.service.skeleton.TokenService;
import com.fasterxml.jackson.databind.ObjectMapper;
import java.math.BigDecimal;
import java.util.UUID;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.ArgumentCaptor;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

@ExtendWith(MockitoExtension.class)
class TokenApiImplTest {

    private static final String TOKEN_ID = "token";
    private static final UUID USER_ID = UUID.randomUUID();

    @Mock private TokenService tokenService;
    @InjectMocks private TokenApiImpl tokenApi;

    private AppExceptionHandler appExceptionHandler = new AppExceptionHandler();
    private MockMvc mockMvc;

    @BeforeEach
    public void setUp() {
        mockMvc =
                MockMvcBuilders.standaloneSetup(tokenApi)
                        .setControllerAdvice(appExceptionHandler)
                        .build();
    }

    @Test
    @DisplayName("Successfully validated existing token")
    public void validateExistingToken() throws Exception {
        SchemasTokenData tokenData = new SchemasTokenData();
        tokenData.data(new SchemasTokenDataData());
        tokenData.getData().setToken(TOKEN_ID);
        when(tokenService.retrieveToken(TOKEN_ID)).thenReturn(tokenData);

        mockMvc.perform(
                        MockMvcRequestBuilders.get(
                                String.format("/api/v1/token/%s/validate", TOKEN_ID)))
                .andExpect(MockMvcResultMatchers.status().isOk())
                .andExpect(MockMvcResultMatchers.content().contentType(MediaType.APPLICATION_JSON))
                .andExpect(MockMvcResultMatchers.jsonPath("$.data.token").value(TOKEN_ID));
    }

    @Test
    @DisplayName("When token does not found, then 404 error thrown")
    public void tokenNotFound() throws Exception {
        when(tokenService.retrieveToken(TOKEN_ID)).thenThrow(TokenNotFoundException.class);

        mockMvc.perform(
                        MockMvcRequestBuilders.get(
                                String.format("/api/v1/token/%s/validate", TOKEN_ID)))
                .andExpect(MockMvcResultMatchers.status().isNotFound())
                .andExpect(MockMvcResultMatchers.content().contentType(MediaType.APPLICATION_JSON))
                .andExpect(MockMvcResultMatchers.jsonPath("$.success").value(false))
                .andExpect(MockMvcResultMatchers.jsonPath("$.status").value(404))
                .andExpect(MockMvcResultMatchers.jsonPath("$.error").value("Not Found"));
    }

    @Test
    @DisplayName("Delete user's token")
    public void deleteUserToken() throws Exception {
        mockMvc.perform(
                        MockMvcRequestBuilders.delete("/api/v1/token/user")
                                .accept(MediaType.APPLICATION_JSON)
                                .header("X-USER-ID", USER_ID.toString()))
                .andExpect(MockMvcResultMatchers.status().isNoContent());

        verify(tokenService, atMostOnce()).deleteUserTokens(USER_ID);
    }

    @Test
    @DisplayName("Delete user's token")
    public void deleteUserTokenFailsWhenNoUserId() throws Exception {
        doThrow(new BadRequestException("Undefined userId."))
                .when(tokenService)
                .deleteUserTokens(null);
        mockMvc.perform(
                        MockMvcRequestBuilders.delete("/api/v1/token/user")
                                .accept(MediaType.APPLICATION_JSON)
                                .header("X-USER-ID", ""))
                .andExpect(MockMvcResultMatchers.status().isBadRequest())
                .andExpect(MockMvcResultMatchers.jsonPath("$.success").value(false))
                .andExpect(MockMvcResultMatchers.jsonPath("$.status").value(400))
                .andExpect(MockMvcResultMatchers.jsonPath("$.error").value("Undefined userId."));
    }

    @Test
    @DisplayName("Successfully created user's token")
    public void shouldCreateToken() throws Exception {
        SchemasGenerateToken token = new SchemasGenerateToken();
        token.setTokenType(SchemasTokenType.FORGOT_PASSWORD);
        token.setTtl(new BigDecimal(100));
        ArgumentCaptor<SchemasGenerateToken> captor =
                ArgumentCaptor.forClass(SchemasGenerateToken.class);
        SchemasGeneratedToken generatedToken = new SchemasGeneratedToken();
        when(tokenService.generateToken(eq(USER_ID), any(SchemasGenerateToken.class)))
                .thenReturn(generatedToken);

        mockMvc.perform(
                        post("/api/v1/token")
                                .header("X-USER-ID", USER_ID.toString())
                                .contentType(MediaType.APPLICATION_JSON)
                                .content(new ObjectMapper().writeValueAsString(token)))
                .andExpect(MockMvcResultMatchers.status().isOk());

        verify(tokenService, atLeastOnce()).generateToken(eq(USER_ID), captor.capture());
        SchemasGenerateToken actualValue = captor.getValue();

        assertEquals(
                actualValue.getTokenType().getValue(), SchemasTokenType.FORGOT_PASSWORD.getValue());
    }

    @Test
    @DisplayName("Successfully deleted token")
    public void deleteToken() throws Exception {
        mockMvc.perform(MockMvcRequestBuilders.delete(String.format("/api/v1/token/%s", TOKEN_ID)))
                .andExpect(MockMvcResultMatchers.status().isNoContent());
    }
}