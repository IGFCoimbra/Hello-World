//
//  Copyright 2020 Autodesk, Inc.  All rights reserved.
//
//  This computer source code and related instructions and comments are the unpublished
//  confidential and proprietary information of Autodesk, Inc. and are protected under
//  applicable copyright and trade secret law. They may not be disclosed to, copied or
//  used by any third party without the prior written consent of Autodesk, Inc.
//

#include "atf_precompile.h"

#include "atf_catv5_producer_impl.h"
#include "atf_catv5_pmi_util.h"
#include "atf_catv5_util.h"

using namespace ATF;
using namespace std;

PMIStandardTypeEnum CATV5PMIUtil::GetPMIStandardType(CC5TPSSet* pTPS)
{
    if (!pTPS)
        return kPMIStandardTypeEnum_Unknown;

    char* standardName = nullptr;
    pTPS->GetTPSDrawStandard(standardName);
    if (standardName == nullptr)
        return kPMIStandardTypeEnum_Unknown;

    PMIStandardTypeEnum standardType = kPMIStandardTypeEnum_Unknown;
    AString standardTypeName(standardName);
    // According to CCE, "CER" and "CEG1" are custom standards 
    // which are created from ISO (parent standard)
    // #4100 will provide the APIs on Dec 29, 2017
    size_t pos = 0;
    if (standardTypeName.Find("ISO", 0, pos) == kIsValueSpecified_Yes
        || standardTypeName.Find("CER", 0, pos)  == kIsValueSpecified_Yes
        || standardTypeName.Find("CEG1", 0, pos)  == kIsValueSpecified_Yes )
        standardType = kPMIStandardTypeEnum_ISO;
    else if (standardTypeName.Find("ANSI", 0, pos)  == kIsValueSpecified_Yes)
        standardType = kPMIStandardTypeEnum_ANSI;
    else if (standardTypeName.Find("ASME", 0, pos)  == kIsValueSpecified_Yes)
        standardType = kPMIStandardTypeEnum_ASME;
    else if (standardTypeName.Find("JIS", 0, pos)  == kIsValueSpecified_Yes)
        standardType = kPMIStandardTypeEnum_JIS;
    else
        ATF_WARNING_ASSERT(0 && "Unknown standard type is found!");

    CC5MemoryDelete_ThreadSafe((void**)&standardName);
    return standardType;
}

// For ISO representation, leader end point will point to center of text
// For ISO representation, leader end point will point to (left/right) bottom of text
bool CATV5PMIUtil::IsSameAsISORepresentation(PMIStandardTypeEnum standardType)
{
    switch (standardType)
    {
        case kPMIStandardTypeEnum_ASME:
            return false;
        case kPMIStandardTypeEnum_ANSI:
        case kPMIStandardTypeEnum_ISO:
        case kPMIStandardTypeEnum_JIS:
            return true;
        case kPMIStandardTypeEnum_Unknown:
        default:
        {
            ATF_WARNING_ASSERT(0 && "Unknown standard type is found!");
            break;
        }
    }

    return true;
}

ATF::ObjectId CATV5PMIUtil::AnnotationObjectId(CC5TPSShape* pShape, const ATF::ObjectId& parentId)
{
    ObjectId annotationObjId;
    if (!pShape)
        return annotationObjId;

    CC5_TPS_TYPE type = CC5_TPS_UNKNOWN;
    CC5_ERROR err = pShape->GetTPSType(type);
    if (err != CC5_QUERY_SUCCESS || type == CC5_TPS_UNKNOWN)
        return annotationObjId;

    switch (type)
    {
        case CC5_TPS_TEXT:
        {
            CC5TPSText* pAnnotationObj = dynamic_cast<CC5TPSText*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSText>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_FLAG_NOTE:
        {
            CC5TPSFlagNote* pAnnotationObj = dynamic_cast<CC5TPSFlagNote*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSFlagNote>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_LINEAR_DIMENSION:
        {
            CC5TPSLinearDimension* pAnnotationObj = dynamic_cast<CC5TPSLinearDimension*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSLinearDimension>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_COORDINATE_DIMENSION:
        {
            CC5TPSCoordDimension* pAnnotationObj = dynamic_cast<CC5TPSCoordDimension*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSCoordDimension>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_GEOMETRIC_TOLERANCE:
        {
            CC5TPSGeometricTolerance* pAnnotationObj = dynamic_cast<CC5TPSGeometricTolerance*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSGeometricTolerance>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_SIMPLE_DATUM:
        {
            CC5TPSSimpleDatum* pAnnotationObj = dynamic_cast<CC5TPSSimpleDatum*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSSimpleDatum>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_DATUM_TARGET:
        {
            CC5TPSDatumTarget* pAnnotationObj = dynamic_cast<CC5TPSDatumTarget*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSDatumTarget>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_ROUGHNESS:
        {
            CC5TPSRoughness* pAnnotationObj = dynamic_cast<CC5TPSRoughness*>(pShape);
            if (!pAnnotationObj)
                return annotationObjId;

            CATV5Util::CATV5ObjectId<CC5TPSRoughness>(pAnnotationObj, parentId, annotationObjId);
            break;
        }
        case CC5_TPS_ANNOT_SET:
        case CC5_TPS_PROJECTED_VIEW:
        case CC5_TPS_REFERENCE_FRAME:
        case CC5_TPS_LEADER:
        case CC5_TPS_WELD_SYMBOL:
        case CC5_TPS_CAPTURE:
        case CC5_TPS_UNKNOWN:
        default:
        {
            ATF_WARNING_ASSERT(0 && "Unsupported pmi type!");
            return annotationObjId;
        }
    }

    annotationObjId.Append("_callout");
    return annotationObjId;
}

bool CATV5PMIUtil::IsAnnotationVisible(CC5TPSShape* pShape)
{
    if (!pShape)
        return false;

    CC5_TPS_TYPE type = CC5_TPS_UNKNOWN;
    CC5_ERROR err = pShape->GetTPSType(type);
    if (err != CC5_QUERY_SUCCESS || type == CC5_TPS_UNKNOWN)
        return false;

    int bVisible = 1;
    switch (type)
    {
    case CC5_TPS_TEXT:
    {
        CC5TPSText* pAnnotationObj = dynamic_cast<CC5TPSText*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);
        break;
    }
    case CC5_TPS_FLAG_NOTE:
    {
        CC5TPSFlagNote* pAnnotationObj = dynamic_cast<CC5TPSFlagNote*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);

        break;
    }
    case CC5_TPS_LINEAR_DIMENSION:
    {
        CC5TPSLinearDimension* pAnnotationObj = dynamic_cast<CC5TPSLinearDimension*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);

        break;
    }
    case CC5_TPS_COORDINATE_DIMENSION:
    {
        CC5TPSCoordDimension* pAnnotationObj = dynamic_cast<CC5TPSCoordDimension*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);

        break;
    }
    case CC5_TPS_GEOMETRIC_TOLERANCE:
    {
        CC5TPSGeometricTolerance* pAnnotationObj = dynamic_cast<CC5TPSGeometricTolerance*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);

        break;
    }
    case CC5_TPS_SIMPLE_DATUM:
    {
        CC5TPSSimpleDatum* pAnnotationObj = dynamic_cast<CC5TPSSimpleDatum*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);

        break;
    }
    case CC5_TPS_DATUM_TARGET:
    {
        CC5TPSDatumTarget* pAnnotationObj = dynamic_cast<CC5TPSDatumTarget*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);

        break;
    }
    case CC5_TPS_ROUGHNESS:
    {
        CC5TPSRoughness* pAnnotationObj = dynamic_cast<CC5TPSRoughness*>(pShape);
        if (!pAnnotationObj)
            return false;

        pAnnotationObj->IsVisible(bVisible);

        break;
    }
    case CC5_TPS_ANNOT_SET:
    case CC5_TPS_PROJECTED_VIEW:
    case CC5_TPS_REFERENCE_FRAME:
    case CC5_TPS_LEADER:
    case CC5_TPS_WELD_SYMBOL:
    case CC5_TPS_CAPTURE:
    case CC5_TPS_UNKNOWN:
    default:
    {
        ATF_WARNING_ASSERT(0 && "Unsupported pmi type!");
        return false;
    }
    }

    return bVisible == 1 ? true : false;
}

void CATV5PMIUtil::GetNearestPoint(CC5TPSShape* pCC5Shape
    , const RoughnessUtilData& roughnessUtilData
    , bool bSymbolMode
    , bool bVersionHigherThanV5R18
    , Point3d& nearestPt
    , int& nIndexOfNearestPt)
{
    if (!pCC5Shape)
        return;

    CC5TPSLeader* pCC5Leader = dynamic_cast<CC5TPSLeader*>(pCC5Shape);
    if (!pCC5Leader)
        return;

    double leaderPos[6];
    CC5_ERROR err = pCC5Leader->GetTPSLeaderPosition(leaderPos);
    if (err != CC5_QUERY_SUCCESS)
        return;

    Point3d leftPt = roughnessUtilData.leftBottomPosition;
    Point3d rightPt = roughnessUtilData.rightBottomPosition;
    Position leaderStartPt(leaderPos[0], leaderPos[1], leftPt.z);
    Position atfLeftPt(leftPt.x, leftPt.y, leftPt.z);
    Position atfRightPt(rightPt.x, rightPt.y, rightPt.z);

    // break points
    int nBreakPts = 0;
    err = pCC5Leader->GetNumberOfBreakPoints(nBreakPts);
    if (err == CC5_QUERY_SUCCESS  && nBreakPts > 0)
    {
        double breakPts[2] = { 0.0 };

        err = pCC5Leader->GetBreakPointAt(breakPts, nBreakPts - 1);
        if (err == CC5_QUERY_SUCCESS)
        {
            leaderStartPt.SetX(breakPts[0]);
            leaderStartPt.SetY(breakPts[1]);
            leaderStartPt.SetZ(leftPt.z);
        }
    }

    //In lower version of CATIA(upto V5R18), for ANSI and ASME standards, there are only two
    //possible connection points, positioned in middle - left and middle - right of the roughness frame
    if (bVersionHigherThanV5R18)
    {
        Position atfPtBetweenLR; // the point between left bottom point and right bottom point
        if (bSymbolMode)
        {
            atfPtBetweenLR.SetX(roughnessUtilData.framePt2.x);
            atfPtBetweenLR.SetY(roughnessUtilData.framePt2.y);
            atfPtBetweenLR.SetZ(roughnessUtilData.framePt2.z);
        }
        else
        {
            atfPtBetweenLR.SetX(roughnessUtilData.middleBottomPosition.x);
            atfPtBetweenLR.SetY(roughnessUtilData.middleBottomPosition.y);
            atfPtBetweenLR.SetZ(roughnessUtilData.middleBottomPosition.z);
        }

        double dDis1 = (atfLeftPt - leaderStartPt).Len();
        double dDis2 = (atfPtBetweenLR - leaderStartPt).Len();
        double dDis3 = (atfRightPt - leaderStartPt).Len();
        if (MathUtil::IsLessThan(dDis1, dDis2))
        {
            if (MathUtil::IsLessThan(dDis1, dDis3))
            {
                nearestPt = Point3d(atfLeftPt.X(), atfLeftPt.Y(), atfLeftPt.Z());
                nIndexOfNearestPt = 0;
            }
            else
            {
                if (MathUtil::IsLessThan(dDis2, dDis3))
                {
                    nearestPt = Point3d(atfPtBetweenLR.X(), atfPtBetweenLR.Y(), atfPtBetweenLR.Z());
                    nIndexOfNearestPt = 1;
                }
                else
                {
                    nearestPt = Point3d(atfRightPt.X(), atfRightPt.Y(), atfRightPt.Z());
                    nIndexOfNearestPt = 2;
                }
            }
        }
        else
        {
            if (MathUtil::IsLessThan(dDis2, dDis3))
            {
                nearestPt = Point3d(atfPtBetweenLR.X(), atfPtBetweenLR.Y(), atfPtBetweenLR.Z());
                nIndexOfNearestPt = 1;
            }
            else
            {
                if (MathUtil::IsLessThan(dDis1, dDis3))
                {
                    nearestPt = Point3d(atfLeftPt.X(), atfLeftPt.Y(), atfLeftPt.Z());
                    nIndexOfNearestPt = 0;
                }
                else
                {
                    nearestPt = Point3d(atfRightPt.X(), atfRightPt.Y(), atfRightPt.Z());
                    nIndexOfNearestPt = 2;
                }
            }
        }
    }
    else
    {
        double dDis1 = (atfLeftPt - leaderStartPt).Len();
        double dDis2 = (atfRightPt - leaderStartPt).Len();
        if (MathUtil::IsLessThan(dDis1, dDis2))
        {
            nearestPt = Point3d(atfLeftPt.X(), atfLeftPt.Y(), atfLeftPt.Z());
            nIndexOfNearestPt = 0;
        }
        else
        {
            nearestPt = Point3d(atfRightPt.X(), atfRightPt.Y(), atfRightPt.Z());
            nIndexOfNearestPt = 2;
        }
    }
}

// GeometryReferenceBuilder
GeometryReferenceBuilder::GeometryReferenceBuilder(CC5Entity* cc5AssoEnt, CC5Part* cc5Part)
    : m_cc5AssoEnt(cc5AssoEnt)
    , m_cc5Part(cc5Part)
{
    for (auto e : CATV5ProducerImpl::Get()->TranslatableGroups())
    {
        if (e && e->GetType() == CC5_SOLIDGROUP_TYPE)
            m_finalBodyList.push_back(e);
        else
            m_othertranslatablegrps.push_back(e);
    }
}

GeometryReferenceBuilder::~GeometryReferenceBuilder()
{}

bool GeometryReferenceBuilder::ReferencedGeometryIds(vector<int>& ids)
{
    if (nullptr == m_cc5AssoEnt || nullptr == m_cc5Part)
        return false;

    ProcessAssociatedGeomEntity(m_cc5AssoEnt, m_cc5Part, ids);
    return true;
}

void GeometryReferenceBuilder::CheckFacesInFinalBody(CC5Face* pFace, CC5Part* pPart, int nType, ENTITIESINFINALSOLID& entities)
{
    if (nullptr == pFace || nullptr == pPart)
        return;

    // Method to check availability of the entity in Final translatable bodies.
    int nFinalBodyID = CheckForEntityInFinalBody(pFace, nType);
    if (nFinalBodyID != 0)
    {
        entities.push_back(pFace);
    }
    else
    {
        // Method to find the resolved entities equivalent to the associated entity
        // All the resolved enitities will be available in the vector "entitiesinfinalsolid"
        GetResolvedFaces(pFace, entities, pPart);
    }
}

void GeometryReferenceBuilder::CheckEdgesInFinalBody(CC5CurveSegment* pCurve, CC5Part* pPart, ENTITIESINFINALSOLID& entities)
{
    if (nullptr == pCurve || nullptr == pPart)
        return;

    int nFinalBodyID = CheckForEntityInFinalBody(pCurve, 1);
    if (nFinalBodyID != 0)
    {
        entities.push_back(pCurve);
    }
    else
    {
        GetResolvedEdges(pCurve, entities, pPart);
    }
}

// To get the entity id of annotation on a face/ edge
// Pass the entity pointer returned by method GetAssociatedGeoEntity(...) in different CC5TPSShape classes.
void GeometryReferenceBuilder::ProcessAssociatedGeomEntity(CC5Entity* Ent, CC5Part* Part, std::vector<int>& ids)
{
    if (nullptr == Ent || nullptr == Part)
        return;

    // Vector to hold all the entities found in the final translatable solid
    ENTITIESINFINALSOLID entitiesinfinalsolid; 
    auto* groupEnt = dynamic_cast<CC5Group*>(Ent->GetParent());
    if (groupEnt && groupEnt->NeedTranslate() == 1) // Is group a translatable entity?
    {
        entitiesinfinalsolid.push_back(groupEnt);
    }
    else
    {
        int nType = Ent->GetType();
        switch (nType)
        {
        case CC5_SKIN_TYPE:
        {
            CC5Skin* pSkin = dynamic_cast<CC5Skin*>(Ent);
            if (!pSkin)
                break;

            // Faces
            if (pSkin->GetNumberOfFaces() == 1)
            {
                CC5Face* face = pSkin->GetFaceAt(0);
                CheckFacesInFinalBody(face, Part, 2, entitiesinfinalsolid);
            }
            else
            {
                // The face count is more than one in cases where CATIA considers multiple faces as a single entity in UI.
                // For Eg: The faces of a cylinder.
                int iNumFace = pSkin->GetNumberOfFaces();
                for (int iFaceItr = 0; iFaceItr < iNumFace; iFaceItr++)
                {
                    CC5Face* face = pSkin->GetFaceAt(iFaceItr);
                    CheckFacesInFinalBody(face, Part, 2, entitiesinfinalsolid);
                }
            }
        }
        break;
        case CC5_COMPOSITECURVE_TYPE:
        {
            CC5CompositeCurve* pCompositeCur = dynamic_cast<CC5CompositeCurve*>(Ent);
            if (!pCompositeCur)
                break;

            // Edges
            if (pCompositeCur->GetNumberOfCurveSegments() == 1)
            {
                CC5CurveSegment* crvSeg = pCompositeCur->GetCurveSegmentAt(0);
                CheckEdgesInFinalBody(crvSeg, Part, entitiesinfinalsolid);
            }
            else
            {
                // The number of segments will be more than one in cases where CATIA considers multiple cuve segments as a single entity in UI.
                // For Eg: The edges of a complete circle.
                int iNumSeg = pCompositeCur->GetNumberOfCurveSegments();
                for (int iSegItr = 0; iSegItr < iNumSeg; iSegItr++)
                {
                    CC5CurveSegment* crvSeg = pCompositeCur->GetCurveSegmentAt(iSegItr);
                    CheckEdgesInFinalBody(crvSeg, Part, entitiesinfinalsolid);
                }
            }
        }
        break;
        case CC5_SOLID_TYPE:
        {
            CC5Solid* pSolid = dynamic_cast<CC5Solid*>(Ent);
            if (!pSolid)
                break;

            // When an annotation is associated to a solid feature (Like Pad/Hole/..), ODXCAT5 returns the intermediate geometry (CC5Solid entity) at that level.
            // This CC5Solid cannot be directly mapped to an entity in any final translatable bodies. 
            // The following workflow could be used to find the faces of intermediate body that are still part of any final translatable body.
            int iNumBody = pSolid->GetNumberOfBodies();
            for (int iBodyItr = 0; iBodyItr < iNumBody; iBodyItr++)
            {
                CC5Body* pBody = pSolid->GetBodyAt(iBodyItr);
                if (pBody && pBody->GetType() == CC5_BODY_TYPE)
                {
                    int iNumSkin = pBody->GetNumberOfSkins();
                    for (int iSkinItr = 0; iSkinItr < iNumSkin; iSkinItr++)
                    {
                        CC5Skin* pSkin = pBody->GetSkinAt(iSkinItr);
                        if (pSkin)
                        {
                            int iNumFace = pSkin->GetNumberOfFaces();
                            for (int iFaceItr = 0; iFaceItr < iNumFace; iFaceItr++)
                            {
                                CC5Face* pFace = pSkin->GetFaceAt(iFaceItr);
                                CheckFacesInFinalBody(pFace, Part, 1, entitiesinfinalsolid);
                            }
                        }
                    }
                }
            }
        }
        break;
        case CC5_POINT_TYPE:
        case CC5_POINTONCURVE_TYPE:
        case CC5_POINTONSURFACE_TYPE:
        {
            // For points and vertex
            //if (Ent->GetType() == CC5_POINT_TYPE)
            //{
            //    CC5Point* pPnt = (CC5Point*)Ent;
            //    entitiesinfinalsolid.push_back(Ent);
            //}
            //// Annotation associated to vertex / point on curve will have this entity type. 
            //else if (Ent->GetType() == CC5_POINTONCURVE_TYPE)
            //{
            //    CC5PointOnCurve* pPoc = (CC5PointOnCurve*)Ent;
            //    // When the associated geom entity is a point on curve, it could be found in the translatable entity list
            //    entitiesinfinalsolid.push_back(Ent);
            //
            //    // For annotation on vertex, the associated entity will not point to an entity in geometry translation.
            //    // In such a case, the 3D coordinates at the vertex could be used.
            //    double coord[3];
            //    pPoc->GetCoordinates(coord);
            //
            //}
            //else if (Ent->GetType() == CC5_POINTONSURFACE_TYPE)
            //{
            //    CC5PointOnSurface* pPos = (CC5PointOnSurface*)Ent;
            //    entitiesinfinalsolid.push_back(Ent);
            //}

            const EventManager* pEventManager = CATV5ProducerImpl::Get()->GetEventManager();
            if (pEventManager)
            {
                GeneralException ex("Point reference is not supported in PMI association.");
                EventPtr<ExceptionEvent> event(new ExceptionEvent(ExceptionEvent::kEventType_NoExceptionThrow, ex));
                pEventManager->FireEvent(event.get());
            }
        }
        break;
        default:
            break;
        }
    }

    // Map the entities in the vector "entitiesinfinalsolid" with the corresponding annotation shape (pShape).
    if (entitiesinfinalsolid.empty())
        ids.push_back(Ent->GetID());
    else
    {
        std::set<int> usefulSet;
        for (auto e : entitiesinfinalsolid)
        {
            int id = e->GetID();
            if (usefulSet.find(id) == usefulSet.end())
            {
                ids.push_back(id);
                usefulSet.insert(id);
            }
        }
    }
}

// Steps to get the entity in the final translatable body :
// If there is no geometrical modification to the pointed geometry,  then it will available in the any of the final translatable bodies.
// And it can be mapped directly.
int GeometryReferenceBuilder::CheckForEntityInFinalBody(CC5Entity* AsscEnt, int iType)
{
    if (!AsscEnt)
        return 0;

    for (CC5Group* pGrp : m_othertranslatablegrps)
    {
        if (!pGrp) continue;
        switch (pGrp->GetType())
        {
        case CC5_SURFACEGROUP_TYPE:
        {
            int nEntity = pGrp->GetNumberOfEntities();
            for (int entityIdx = 0; entityIdx < nEntity; entityIdx++)
            {
                CC5Entity* entity = pGrp->GetEntityAt(entityIdx);
                if (!entity)
                    continue;
                if (entity->GetType() == CC5_SKIN_TYPE)
                {
                    CC5Skin* skin = dynamic_cast<CC5Skin*>(entity);
                    if (!skin) continue;
                    int no_of_faces = skin->GetNumberOfFaces();
                    for (int ii = 0; ii < no_of_faces; ii++)
                    {
                        CC5Face* face = skin->GetFaceAt(ii);
                        if (!face)continue;
                        if (iType == 2) {
                            if (face->GetID() == AsscEnt->GetID()) {
                                return pGrp->GetID();
                            }
                        }
                        if (iType == 1) {
                            int nLoops = face->GetNumberOfLoops();
                            for (int iLoop = 0; iLoop < nLoops; iLoop++)
                            {
                                CC5Loop* pLoop = face->GetLoopAt(iLoop);
                                int nEdges = pLoop->GetNumberOfEdges();
                                for (int iEdge = 0; iEdge < nEdges; iEdge++) {
                                    CC5CurveSegment* pCrvSeg = pLoop->GetEdgeAt(iEdge);
                                    if (pCrvSeg->GetID() == AsscEnt->GetID()) {
                                        return pGrp->GetID();
                                    }
                                    CC5ObjectDelete_ThreadSafe((CC5Object**)&pCrvSeg);
                                }
                                CC5ObjectDelete_ThreadSafe((CC5Object**)&pLoop);
                            }
                        }
                        CC5ObjectDelete_ThreadSafe((CC5Object**)&face);
                    }
                }
                CC5ObjectDelete_ThreadSafe((CC5Object**)&entity);
            }
        }
        break;
        case CC5_CURVEGROUP_TYPE:
        {
            int nentity = pGrp->GetNumberOfEntities();
            for (int entitycount = 0; entitycount < nentity; entitycount++)
            {
                CC5Entity *entity = pGrp->GetEntityAt(entitycount);
                if (!entity)
                    continue;
                if (entity->GetType() == CC5_COMPOSITECURVE_TYPE)
                {
                    CC5CompositeCurve *compCurve = dynamic_cast<CC5CompositeCurve*>(entity);
                    int edge_count = compCurve->GetNumberOfCurveSegments();
                    for (int l = 0; l < edge_count; l++)
                    {
                        CC5CurveSegment* edge = compCurve->GetCurveSegmentAt(l);
                        if (!edge) continue;
                        if (edge->GetID() == AsscEnt->GetID()) {
                            return pGrp->GetID();
                        }
                        CC5ObjectDelete_ThreadSafe((CC5Object**)&edge);
                    }
                }
                CC5ObjectDelete_ThreadSafe((CC5Object**)&entity);
            }
        }
        break;
        default:
            break;
        }
    }

    FINALBODYLIST::iterator FinalBodyItr = m_finalBodyList.begin();
    while (FinalBodyItr != m_finalBodyList.end())
    {
        CC5Group* pGrp = *FinalBodyItr;
        if (!pGrp) continue;
        if (pGrp->GetType() == CC5_SOLIDGROUP_TYPE)
        {
            int iNumEnt = pGrp->GetNumberOfEntities();
            for (int iEntItr = 0; iEntItr < iNumEnt; iEntItr++)
            {
                CC5Entity* pEnt = pGrp->GetEntityAt(iEntItr);
                if (!pEnt) continue;
                if (pEnt->GetType() == CC5_SOLID_TYPE)
                {
                    int iNumBody = (dynamic_cast<CC5Solid*>(pEnt))->GetNumberOfBodies();
                    for (int iBodyItr = 0; iBodyItr < iNumBody; iBodyItr++)
                    {
                        CC5Body* pBody = (dynamic_cast<CC5Solid*>(pEnt))->GetBodyAt(iBodyItr);
                        if (!pBody) continue;
                        if (pBody && pBody->GetType() == CC5_BODY_TYPE)
                        {
                            int iNumSkin = pBody->GetNumberOfSkins();
                            for (int iSkinItr = 0; iSkinItr < iNumSkin; iSkinItr++)
                            {
                                CC5Skin* pSkin = pBody->GetSkinAt(iSkinItr);
                                if (!pSkin) continue;
                                int iNumFace = pSkin->GetNumberOfFaces();
                                for (int iFaceItr = 0; iFaceItr < iNumFace; iFaceItr++)
                                {
                                    CC5Face* pFace = pSkin->GetFaceAt(iFaceItr);
                                    if (!pFace) continue;
                                    if (iType == 2) {
                                        if (pFace->GetID() == AsscEnt->GetID()) {
                                            return pGrp->GetID();
                                        }
                                    }
                                    if (iType == 1) {
                                        int loop_count = pFace->GetNumberOfLoops();
                                        for (int k = 0; k < loop_count; k++)
                                        {
                                            CC5Loop* loop = pFace->GetLoopAt(k);
                                            if (!loop) continue;
                                            int edge_count = loop->GetNumberOfEdges();
                                            for (int l = 0; l < edge_count; l++)
                                            {
                                                CC5CurveSegment* edge = loop->GetEdgeAt(l);
                                                if (!edge) continue;
                                                if (edge->GetID() == AsscEnt->GetID()) {
                                                    return pGrp->GetID();
                                                }
                                                CC5ObjectDelete_ThreadSafe((CC5Object**)&edge);
                                            }
                                            CC5ObjectDelete_ThreadSafe((CC5Object**)&loop);
                                        }
                                    }
                                    CC5ObjectDelete_ThreadSafe((CC5Object **)&pFace);
                                }
                                CC5ObjectDelete_ThreadSafe((CC5Object **)&pSkin);
                            }
                        }
                        CC5ObjectDelete_ThreadSafe((CC5Object **)&pBody);
                    }
                }
                CC5ObjectDelete_ThreadSafe((CC5Object **)&pEnt);
            }
        }
        ++FinalBodyItr;
    }
    return 0;
}

// Method to get the final resolved face/faces in the translatable body
// A face in the intermediate body can be split into multiple faces in the final body.
// All the resolved faces will be available in the vector "entitiesinfinalsolid"
int GeometryReferenceBuilder::GetResolvedFaces(CC5Entity* asscEnt, ENTITIESINFINALSOLID& entitiesinfinalsolid, CC5Part* Part)
{
    if (!asscEnt || !Part)
        return 0;

    int finalBodyID = 0;
    //int iIntermediateBodyID = 0;   //codacy reported Variable 'iIntermediateBodyID' is assigned a value that is never used.
    CC5Entity* pIntermdtEnt1 = nullptr;
    CC5Entity* pIntermdtEnt2 = nullptr;

    int pParentType = 0;
    CC5Entity* pParent = asscEnt->GetParent();
    pParent = pParent->GetParent();
    if (pParent)
        pParentType = pParent->GetType();

    if (pParentType != CC5_BODY_TYPE)
        /*iIntermediateBodyID = */FindAsscEntityInIntermediateSolid(asscEnt, 2, Part, pIntermdtEnt1, pIntermdtEnt2);

    if (pParentType == CC5_BODY_TYPE)
        pIntermdtEnt1 = asscEnt;

    finalBodyID = FindEntityUsingGeomIDs(pIntermdtEnt1, nullptr, 2, entitiesinfinalsolid);

    if (finalBodyID)
        return finalBodyID;

    return 0;
}

// Method to get the final resolved edge/edges in the translatable body
// An edge in the intermediate body can be split into multiple edges in the final body.
// All the resolved edges will be available in the vector "entitiesinfinalsolid"
int GeometryReferenceBuilder::GetResolvedEdges(CC5Entity* asscEnt, ENTITIESINFINALSOLID& entitiesinfinalsolid, CC5Part* Part)
{
    if (!asscEnt || !Part)
        return 0;

    int finalBodyID = 0;
    CC5Entity* pIntermdtEnt1 = nullptr;
    CC5Entity* pIntermdtEnt2 = nullptr;
    int iIntermediateBodyID = FindAsscEntityInIntermediateSolid(asscEnt, 1, Part, pIntermdtEnt1, pIntermdtEnt2);
    if (iIntermediateBodyID != 0)
        finalBodyID = FindEntityUsingGeomIDs(pIntermdtEnt1, pIntermdtEnt2, 1, entitiesinfinalsolid);
    if (finalBodyID)
        return finalBodyID;

    return 0;
}

// Method to get the entity(Face/Edge) in the intermediate body: pass iType = 2 for face, iType = 1 for edge
// For face, pIntermdtEnt1 will give resolved face.
// For edge, this method will return two sharing faces from pIntermdtEnt1 and pIntermdtEnt2
int GeometryReferenceBuilder::FindAsscEntityInIntermediateSolid(CC5Entity* asscEnt, int iType, CC5Part* Part, CC5Entity*& pIntermdtEnt1, CC5Entity*& pIntermdtEnt2)
{
    if (!asscEnt || !Part)
        return 0;

    bool bEntFound = false;
    int nGrps = Part->GetNumberOfGroups();
    //bool intermediateface_found = false;
    for (int i = 0; i < nGrps; i++)
    {
        CC5Group* pGrp = Part->GetGroupAt(i);
        //if (!pGrp) 
        //    continue;
        if (pGrp->GetType() == CC5_SOLIDGROUP_TYPE)
        {
            int iNumEnt = pGrp->GetNumberOfEntities();
            for (int iEntItr = 0; iEntItr < iNumEnt; iEntItr++)
            {
                CC5Entity* pEnt = pGrp->GetEntityAt(iEntItr);
                if (!pEnt) continue;
                if (pEnt->GetType() == CC5_SOLID_TYPE)
                {
					CC5Solid * pCC5SolidBody = dynamic_cast<CC5Solid*>(pEnt);
					if (!pCC5SolidBody)
						continue;

                    int iNumBody = pCC5SolidBody->GetNumberOfBodies();
                    for (int iBodyItr = 0; iBodyItr < iNumBody; iBodyItr++)
                    {
                        CC5Body* pBody = pCC5SolidBody->GetBodyAt(iBodyItr);
                        if (!pBody) continue;
                        if (pBody && pBody->GetType() == CC5_BODY_TYPE)
                        {
                            int iNumSkin = pBody->GetNumberOfSkins();
                            for (int iSkinItr = 0; iSkinItr < iNumSkin; iSkinItr++)
                            {
                                CC5Skin* pSkin = pBody->GetSkinAt(iSkinItr);
                                if (!pSkin) continue;
                                int iNumFace = pSkin->GetNumberOfFaces();
                                for (int iFaceItr = 0; iFaceItr < iNumFace; iFaceItr++)
                                {
                                    CC5Face* pFace = pSkin->GetFaceAt(iFaceItr);
                                    if (!pFace) continue;
                                    if (iType == 2 && pFace->GetID() == asscEnt->GetID()) {
                                        if (pFace->GetID() == asscEnt->GetID()) {
                                            pIntermdtEnt1 = pFace;
                                            return pGrp->GetID();
                                        }
                                    }
                                    else if (iType == 1) {
                                        int loop_count = pFace->GetNumberOfLoops();
                                        for (int k = 0; k < loop_count; k++)
                                        {
                                            CC5Loop* loop = pFace->GetLoopAt(k);
                                            if (!loop) continue;
                                            int edge_count = loop->GetNumberOfEdges();
                                            for (int l = 0; l < edge_count; l++)
                                            {
                                                CC5CurveSegment* edge = loop->GetEdgeAt(l);
                                                if (!edge) continue;
                                                if (edge->GetID() != asscEnt->GetID())
                                                    continue;
                                                if (edge->CoEdgeExisted() == CC5_TRUE) {
                                                    if (pIntermdtEnt1 == nullptr)
                                                        pIntermdtEnt1 = pFace;
                                                    else
                                                        pIntermdtEnt2 = pFace;
                                                    if (pIntermdtEnt1 && pIntermdtEnt2)
                                                        return pGrp->GetID();
                                                    bEntFound = true;
                                                    continue;
                                                }
                                                if (bEntFound == true) continue;
                                                CC5ObjectDelete_ThreadSafe((CC5Object**)&edge);
                                            }
                                            if (bEntFound == true) continue;
                                            CC5ObjectDelete_ThreadSafe((CC5Object**)&loop);
                                        }
                                    }
                                    if (bEntFound == true) continue;
                                    CC5ObjectDelete_ThreadSafe((CC5Object**)&pFace);
                                }
                                if (bEntFound == true) continue;
                                CC5ObjectDelete_ThreadSafe((CC5Object**)&pSkin);
                            }
                        }
                        if (bEntFound == true) continue;
                        CC5ObjectDelete_ThreadSafe((CC5Object**)&pBody);
                    }
                }
                if (bEntFound == true) continue;
                CC5ObjectDelete_ThreadSafe((CC5Object**)&pEnt);
            }
        }
    }
    return 0;
}

// Method to find the equivalent faces of the associated entity
// Use method CC5Face::GetPersistentIdentifier(...) to identify the faces
// Note :
//      In case of an edge, the PersistentIdentifier of the sharing faces of edge could be used.
int GeometryReferenceBuilder::FindEntityUsingGeomIDs(CC5Entity* pIntermdtEnt1, CC5Entity* pIntermdtEnt2, int iType, ENTITIESINFINALSOLID& entitiesinfinalsolid)
{
    size_t iSize = entitiesinfinalsolid.size();
    EDGE_FACE edge_face;
    CC5Face* asscFace = nullptr;
    if (iType == 2)
        asscFace = dynamic_cast<CC5Face*>(pIntermdtEnt1);

    bool bFaceMatched = true;
    FINALBODYLIST::iterator finalBodyItr;
    finalBodyItr = m_finalBodyList.begin();
    while (finalBodyItr != m_finalBodyList.end())
    {
        CC5Group* pGrp = *finalBodyItr;
        if (!pGrp) continue;
        if (pGrp->GetType() == CC5_SOLIDGROUP_TYPE)
        {
            int iNumEnt = pGrp->GetNumberOfEntities();
            for (int iEntItr = 0; iEntItr < iNumEnt; iEntItr++)
            {
                CC5Entity* pEnt = pGrp->GetEntityAt(iEntItr);
                if (!pEnt) continue;
                if (pEnt->GetType() == CC5_SOLID_TYPE)
                {
					CC5Solid * pCC5SolidBody = dynamic_cast<CC5Solid*>(pEnt);
					if (!pCC5SolidBody)
						continue;

                    int iNumBody = pCC5SolidBody->GetNumberOfBodies();
                    for (int iBodyItr = 0; iBodyItr < iNumBody; iBodyItr++)
                    {
                        CC5Body* pBody = pCC5SolidBody->GetBodyAt(iBodyItr);
                        if (!pBody) continue;
                        if (pBody && pBody->GetType() == CC5_BODY_TYPE)
                        {
                            int iNumSkin = pBody->GetNumberOfSkins();
                            for (int iSkinItr = 0; iSkinItr < iNumSkin; iSkinItr++)
                            {
                                CC5Skin* pSkin = pBody->GetSkinAt(iSkinItr);
                                if (!pSkin) continue;
                                int iNumFace = pSkin->GetNumberOfFaces();
                                for (int iFaceItr = 0; iFaceItr < iNumFace; iFaceItr++)
                                {
                                    bFaceMatched = true;
                                    CC5Face* pFace = pSkin->GetFaceAt(iFaceItr);
                                    if (!pFace) continue;
                                    if (iType == 2) {
                                        CheckFaceInFaceGroups(pFace, asscFace, bFaceMatched);
                                        if (bFaceMatched == true) {
                                            entitiesinfinalsolid.push_back(pFace);
                                            continue;
                                        }
                                    }
                                    if (iType == 1) {
                                        int loop_count = pFace->GetNumberOfLoops();
                                        for (int k = 0; k < loop_count; k++)
                                        {
                                            CC5Loop* loop = pFace->GetLoopAt(k);
                                            if (!loop) continue;
                                            int edge_count = loop->GetNumberOfEdges();
                                            for (int l = 0; l < edge_count; l++)
                                            {
                                                CC5CurveSegment* edge = loop->GetEdgeAt(l);
                                                if (!edge) continue;
                                                if (edge->CoEdgeExisted() != CC5_TRUE)
                                                    continue;

                                                EDGE_FACE::iterator edge_face_itr = edge_face.find(edge->GetID());
                                                if (edge_face_itr == edge_face.end()) {
                                                    edge_face.insert(pair<int, CC5Face*>(edge->GetID(), pFace));
                                                }
                                                else {
                                                    edge_face.insert(pair<int, CC5Face*>(edge->GetID(), pFace));

                                                    //EDGE_FACE::iterator edge_face_itr;
                                                    int counter = 0;
                                                    for (int i = 0; i < 2; i++)
                                                    {
                                                        // bool bFaceMatched = true;
                                                        CC5Face* pIntrmdtFace = nullptr;
                                                        if (i == 0)
                                                            pIntrmdtFace = dynamic_cast<CC5Face*>(pIntermdtEnt1);
                                                        else
                                                            pIntrmdtFace = dynamic_cast<CC5Face*>(pIntermdtEnt2);

                                                        edge_face_itr = edge_face.lower_bound(edge->GetID());
                                                        CC5Face* pFinalFace1 = edge_face_itr->second;
                                                        CheckFaceInFaceGroups(pIntrmdtFace, pFinalFace1, bFaceMatched);

                                                        if (bFaceMatched == false) {
                                                            bFaceMatched = true;
                                                            ++edge_face_itr;
                                                            CC5Face* pFinalFace2 = edge_face_itr->second;
                                                            CheckFaceInFaceGroups(pIntrmdtFace, pFinalFace2, bFaceMatched);
                                                        }
                                                        if (bFaceMatched == true) {
                                                            counter++;
                                                            if (counter == 2) entitiesinfinalsolid.push_back(edge);
                                                            continue;
                                                        }
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if (bFaceMatched == true) continue;
                                    CC5ObjectDelete_ThreadSafe((CC5Object**)&pFace);
                                }
                                if (bFaceMatched == true) continue;
                                CC5ObjectDelete_ThreadSafe((CC5Object**)&pSkin);
                            }
                        }
                        if (bFaceMatched == true) continue;
                        CC5ObjectDelete_ThreadSafe((CC5Object**)&pBody);
                    }
                }
                if (bFaceMatched == true) continue;
                CC5ObjectDelete_ThreadSafe((CC5Object**)&pEnt);
            }
        }
        if (entitiesinfinalsolid.size() > iSize)
            return pGrp->GetID();
        ++finalBodyItr;
    }
    return 0;
}

//Method to compare the PersistentIdentifier of the faces
void GeometryReferenceBuilder::CheckFaceInFaceGroups(CC5Face* pFace, CC5Face* asscFace, bool& bFaceMatched)
{
    if (!pFace || !asscFace)
    {
        bFaceMatched = false;
        return;
    }

    CC5PersistentID* pfacePersisID = nullptr;
    CC5PersistentID* pAsscfacePersisID = nullptr;
    pFace->GetPersistentIdentifier(pfacePersisID);
    asscFace->GetPersistentIdentifier(pAsscfacePersisID);
    if (!pfacePersisID) {
        bFaceMatched = false;
        return;
    }
    for (int i = 0; i < pfacePersisID->GetGroupCount(); i++)
    {
        int iSize = 0;
        int* iIDList = nullptr;
        pfacePersisID->GetGroupAt(i, iSize, iIDList);
        if (!pAsscfacePersisID) {
            bFaceMatched = false;
            continue;
        }
        for (int j = 0; j < pAsscfacePersisID->GetGroupCount(); j++)
        {
            bFaceMatched = true;
            int jSize = 0;
            int* jIDList = nullptr;
            pAsscfacePersisID->GetGroupAt(j, jSize, jIDList);
            if (iSize != jSize)
                bFaceMatched = false;
            if (iSize == jSize) {
                for (int k = 0; k < jSize; k++) {
                    if (iIDList[k] != jIDList[k]) {
                        bFaceMatched = false;
                        break;
                    }
                }
                if (bFaceMatched == true) break;
            }
            if (bFaceMatched == true) break;
        }
        if (bFaceMatched == true) break;
    }
}