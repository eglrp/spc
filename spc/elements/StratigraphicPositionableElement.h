#pragma once
#ifndef STRATIGRAPHICPOSITIONABLEELEMENT_H
#define STRATIGRAPHICPOSITIONABLEELEMENT_H

#include <spc/elements/GeologicalElement.h>
#include <spc/elements/StratigraphicModelBase.h>


namespace spc
{


class StratigraphicPositionableElement: public GeologicalElement
{
public:

    spcTypedefSharedPtrs(StratigraphicPositionableElement)
    EXPOSE_TYPE

    StratigraphicPositionableElement()
    {
    }

    StratigraphicPositionableElement(const StratigraphicPositionableElement & other)
    {
        stratigraphic_position_ = other.stratigraphic_position_;
        manual_ = other.manual_;
        strat_model_ = other.strat_model_;
    }

    ~StratigraphicPositionableElement()
    {

    }


//    StratigraphicPositionableElement(const float x, const float y, const float z): Point3D(x,y,z)
//    {

//    }

//    StratigraphicPositionableElement(const Eigen::Vector3f point): Point3D(point)
//    {

//    }


    bool hasModel() const
    {
        if (strat_model_== nullptr)
            return false;
        else
            return true;
    }


    void modelFromParent()
    {
        if (this->getParent()!=nullptr && this->getParent()->isA(&spc::StratigraphicModelBase::Type))
        {
            LOG(INFO) << "loading parent as model";
            strat_model_ = spcDynamicPointerCast<spc::StratigraphicModelBase>(this->getParent());
        }
    }



    spcGetObjectMacro(StratigraphicModel, strat_model_, StratigraphicModelBase)

    void setStratigraphicModel(const StratigraphicModelBase::Ptr & mod)
    {
        if (mod == nullptr)
            return;
        else
            strat_model_ = mod;
    }



    spcGetMacro(Manual, manual_, bool)
    spcSetMacro(Manual, manual_, bool)

    float getSquaredResidual() const
    {
        if (!getManual())
            return 0;
        else
        {
            float out = predictStratigraphicPositionFromModel() - getStratigraphicPosition();
            return out*out;
        }
    }

    /** a stratigraphic positionable element needs a method to predict its position
     * in stratigraphy
     **/
    virtual float predictStratigraphicPositionFromModel () const = 0;

    float getStratigraphicPosition() const
    {
        if ( getManual() ) // in manual mode always give the user chosen sp
            return stratigraphic_position_;

        else if (!hasModel()) // if we dont have a model just return nan;
            return spcNANMacro;

        else // we are not in manual and we have a strat model.
            return predictStratigraphicPositionFromModel();
    }



    void setStratigraphicPosition(const float &sp)
    {
        stratigraphic_position_ = sp;

        LOG(INFO) << "new stratigraphic position manually set to " << stratigraphic_position_ ;
    }





private:
    float stratigraphic_position_ = 0;

    bool manual_ = true;

    StratigraphicModelBase::Ptr strat_model_ = nullptr;



private:
    friend class cereal::access;

    template <class Archive> void serialize(Archive &ar, const std::uint32_t version)
    {
        ar(cereal::base_class<GeologicalElement>(this),
           CEREAL_NVP(stratigraphic_position_),
           CEREAL_NVP(strat_model_),
           CEREAL_NVP(manual_));
    }

};


}// end nspace
#endif // STRATIGRAPHICPOSITIONABLEELEMENT_H


