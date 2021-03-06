//                                               -*- C++ -*-
/**
 *  @brief Analytical implements an algorithm to find the design point
 *
 *  Copyright 2005-2017 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <cmath>

#include "openturns/Analytical.hxx"
#include "openturns/Distribution.hxx"
#include "openturns/ComparisonOperatorImplementation.hxx"
#include "openturns/PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Analytical);

static const Factory<Analytical> Factory_Analytical;

/*
 * @brief  Standard constructor: the class is defined by an optimisation algorithm, a failure event and a physical starting point
 */
Analytical::Analytical(const OptimizationSolver & nearestPointAlgorithm,
                       const Event & event,
                       const NumericalPoint & physicalStartingPoint)
  : PersistentObject(),
    nearestPointAlgorithm_(nearestPointAlgorithm),
    event_(event),
    physicalStartingPoint_(physicalStartingPoint)
{
  const UnsignedInteger dimension = event.getImplementation()->getFunction().getInputDimension();
  if (physicalStartingPoint.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Starting point dimension (" << physicalStartingPoint.getDimension() << ") does not match event dimension (" << dimension << ").";
  result_ = AnalyticalResult(event_.getImplementation()->getAntecedent()->getDistribution().getIsoProbabilisticTransformation().operator()(physicalStartingPoint_), event, true);
  /* set the level function of the algorithm */
  nearestPointAlgorithm_.setProblem(OptimizationProblem(event.getImplementation()->getFunction(), event.getThreshold()));
}


/* Virtual constructor */
Analytical * Analytical::clone() const
{
  return new Analytical(*this);
}

/* Physical starting point accessor */
NumericalPoint Analytical::getPhysicalStartingPoint() const
{
  return physicalStartingPoint_;
}

/* Physical starting point accessor */
void Analytical::setPhysicalStartingPoint(const NumericalPoint & physicalStartingPoint)
{
  physicalStartingPoint_ = physicalStartingPoint;
}

/* Event accessor */
Event Analytical::getEvent() const
{
  return event_;
}

/* Event accessor */
void Analytical::setEvent(const Event & event)
{
  event_ =  event;
  nearestPointAlgorithm_.setProblem(OptimizationProblem(event_.getImplementation()->getFunction(), event_.getThreshold()));
}

/* OptimizationSolver accessor */
OptimizationSolver Analytical::getNearestPointAlgorithm() const
{
  return nearestPointAlgorithm_;
}

/* OptimizationSolver accessor */
void Analytical::setNearestPointAlgorithm(const OptimizationSolver & nearestPointAlgorithm)
{
  nearestPointAlgorithm_ = nearestPointAlgorithm_;
}

/* String converter */
String Analytical::__repr__() const
{
  OSS oss;
  oss << "class=" << Analytical::GetClassName()
      << " nearestPointAlgorithm=" << nearestPointAlgorithm_.__repr__()
      << " event=" << event_
      << " physicalstartingPoint=" << physicalStartingPoint_;
  return oss;
}

/* Performs the actual computation. */
void Analytical::run()
{
  /* Build a standard event from the event */
  StandardEvent standardEvent(event_);

  /* set the level function of the algorithm */
  nearestPointAlgorithm_.setProblem(OptimizationProblem(standardEvent.getImplementation()->getFunction(), standardEvent.getThreshold()));

  /* set the starting point of the algorithm in the standard space  */
  nearestPointAlgorithm_.setStartingPoint(event_.getImplementation()->getAntecedent()->getDistribution().getIsoProbabilisticTransformation().operator()(physicalStartingPoint_));

  /* solve the nearest point problem */
  nearestPointAlgorithm_.run();

  /* store the optimization result into the analytical result */
  result_.setOptimizationResult(nearestPointAlgorithm_.getResult());
  /* set standard space design point in Result */
  NumericalPoint standardSpaceDesignPoint(nearestPointAlgorithm_.getResult().getOptimalPoint());
  standardSpaceDesignPoint.setName("Standard Space Design Point");
  result_.setStandardSpaceDesignPoint(standardSpaceDesignPoint);

  /* set isStandardPointOriginInFailureSpace in Result */
  NumericalPoint origin(standardSpaceDesignPoint.getDimension(), 0.0);
  NumericalPoint value(standardEvent.getImplementation()->getFunction().operator()(origin));

  result_.setIsStandardPointOriginInFailureSpace(event_.getOperator().compare(value[0], event_.getThreshold()));
} /* Analytical::run() */

/* Result accessor */
AnalyticalResult Analytical::getAnalyticalResult() const
{
  return result_;
}


/* Method save() stores the object through the StorageManager */
void Analytical::save(Advocate & adv) const
{

  PersistentObject::save(adv);
  adv.saveAttribute("nearestPointAlgorithm_", nearestPointAlgorithm_);
  adv.saveAttribute("event_", event_);
  adv.saveAttribute("physicalStartingPoint_", physicalStartingPoint_);
  adv.saveAttribute("result_", result_);
}

/* Method load() reloads the object from the StorageManager */
void Analytical::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("nearestPointAlgorithm_", nearestPointAlgorithm_);
  adv.loadAttribute("event_", event_);
  adv.loadAttribute("physicalStartingPoint_", physicalStartingPoint_);
  adv.loadAttribute("result_", result_);
}

END_NAMESPACE_OPENTURNS

