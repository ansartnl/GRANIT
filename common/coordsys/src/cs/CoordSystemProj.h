#ifndef COORD_SYSTEM_PROJ_H
#define COORD_SYSTEM_PROJ_H

#include "cs/CoordSystem.h"
#include <QVector3D>
#include <QPointF>


//! Coordinate conversions using Gauss Kruger projection system.
class MATHTRSHARED_EXPORT CCoordSystemProj : public CCoordSystem
{
public:
    CCoordSystemProj();
    ~CCoordSystemProj();

    virtual QString GetProjectionName() {return "Proj.4";}

    //! Conversion from geodesic to 3D model coordinate.
    /*!
        \param pointGeo Geodesic coordinate: x,y - radians, h - height.
        \return X,Y,Z coordinate in 3D space.
    */
    virtual QVector3D toModel(const QVector3D &pointGeo) const;

    //! Conversion from geodesic to 3D model coordinate.
    /*!
        \param pointGeo Geodesic coordinate: x,y - radians, h - height.
        \return X,Y,Z coordinate in 3D space.
    */
    virtual QVector3D toModelDegree(const QVector3D &pointGeoDegree) const;

    //! Conversion from geodesic to 3D model coordinate.
    /*!
        \param pointGeo Geodesic coordinate: x,y - radians, h - height.
        \return X,Y,Z coordinate in 3D space.
    */
    virtual void toModel(CCoord &coord) const;

    //! Conversion from 3D model to geodesic coordinate.
    /*!
        \param pointModel Coordinate in 3D space.
        \return Geodesic coordinate: x,y - radians, h - height.
    */
    virtual QVector3D toGeo(const QVector3D &pointModel) const;

    //! Conversion from 3D model to geodesic coordinate.
    /*!
        \param pointModel Coordinate in 3D space.
        \return Geodesic coordinate: x,y - degree, h - height.
    */
    virtual QVector3D toGeoDegree(const QVector3D &pointModel) const;

    //! Conversion from 3D model to geodesic coordinate.
    /*!
        \param pointModel Coordinate in 3D space.
        \return Geodesic coordinate: x,y - radians, h - height.
    */
    virtual void toGeo(CCoord &coord) const;

    //! Conversion from scene to 3D model coordinate.
    virtual QVector3D toModel(const QPointF &pointScene) const;

    //! Projection of 3D model coordinate to scene coordinate.
    virtual QPointF toScene(const QVector3D &pointModel) const;

    //! Projection of 3D model coordinate to scene coordinate.
    virtual QPointF toScene(const CCoord &coord) const;

    //! Projection of 3D model coordinate to scene coordinate
    /*! Conversion to scene coordinates without applying rotation
        to coordinate.
    */
    virtual QPointF toSceneNoRotation(const QVector3D &pointModel) const;

    //! Returns true if model is binded to center point.
    /*!
        Gauss Kruger plane coordinates are stored as binded to the center point.
        \note When new center point is set all model coordinates should be
              recalculated.
    */
    virtual bool isModelCentered() const
    { return true; }

    //! Retrieve distance between 2 coordinates.
    virtual qreal distance(const CCoord &coord1, const CCoord &coord2) const;

    //! Define center point which defines center zone and point for conversion.
    void defineCenterPoint(qreal latitude, qreal longitude);

    //! Find difference angle to north direction.
    /*! Find difference between direction to north in squared projection
        and direction to north in current projection

        \param latitude Origin latitude to calculate difference angle
        \param longitude Origin longitude to calculate difference angle
        \return Angle in degree
    */
    virtual qreal findAngleToNorth(qreal latitude, qreal longitude) const;

private:
    typedef void *ProjHandler;
    ProjHandler m_projHandler;

    QVector3D   m_CenterPoint;  //!< Center (anchor) point
    qint32      m_CenterZone;   //!< Center zone number center point belongs to
};

#endif // COORD_SYSTEM_PROJ_H
