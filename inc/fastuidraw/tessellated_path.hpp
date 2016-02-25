/*!
 * \file tessellated_path.hpp
 * \brief file tessellated_path.hpp
 *
 * Copyright 2016 by Intel.
 *
 * Contact: kevin.rogovin@intel.com
 *
 * This Source Code Form is subject to the
 * terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with
 * this file, You can obtain one at
 * http://mozilla.org/MPL/2.0/.
 *
 * \author Kevin Rogovin <kevin.rogovin@intel.com>
 *
 */


#pragma once


#include <fastuidraw/util/fastuidraw_memory.hpp>
#include <fastuidraw/util/vecN.hpp>
#include <fastuidraw/util/c_array.hpp>
#include <fastuidraw/util/reference_counted.hpp>
#include <fastuidraw/stroked_path.hpp>
#include <fastuidraw/filled_path.hpp>

namespace fastuidraw  {

///@cond
class Path;
///@endcond

/*!\addtogroup Core
  @{
 */

/*!
  A TessellatedPath represents the tessellation of a Path.
 */
class TessellatedPath:
    public reference_counted<TessellatedPath>::non_concurrent
{
public:
  /*!
    A TessellationParams stores how finely to tessellate
    the curves of a path.
   */
  class TessellationParams
  {
  public:
    /*!
      Ctor, initializes values.
     */
    TessellationParams(void):
      m_curve_tessellation(float(M_PI)/30.0f),
      m_max_segments(32)
    {}

    /*!
      Non-equal comparison operator.
      \param rhs value to which to compare against
     */
    bool
    operator!=(const TessellationParams &rhs) const
    {
      return m_curve_tessellation != rhs.m_curve_tessellation
        || m_max_segments != rhs.m_max_segments;
    }

    /*!
      For each 2 PI units of curvature, goal is to have
      atleast m_curve_tessellation points, initial
      value is PI/30, i.e. 60 points per 2 PI curvature,
      i.e. a circle would be tessellated to 60 points.
     */
    float m_curve_tessellation;

    /*!
      Maximum number of segments to tessellate each
      PathContour::interpolator_base from each
      PathContour of a Path.
     */
    unsigned int m_max_segments;
  };

  /*!
    Represents point of a tessellated path.
   */
  class point
  {
  public:
    /*!
      The position of the point.
     */
    vec2 m_p;

    /*!
      The derivative of the curve at the point.
     */
    vec2 m_p_t;

    /*!
      Gives the distance of the point from the start
      of the -edge- on which the point resides.
     */
    float m_distance_from_edge_start;

    /*!
      Gives the distance of the point from the start
      of the -outline- on which the point resides.
     */
    float m_distance_from_outline_start;
  };

  /*!
    Ctor. Construct a TessellatedPath from a Path
    \param input source path to tessellate
    \param P parameters on how to tessellate the source Path
   */
  TessellatedPath(const Path &input, TessellationParams P);

  ~TessellatedPath();

  /*!
    Returns the tessellation parameters used to construct
    this TessellatedPath.
   */
  const TessellationParams&
  tessellation_parameters(void) const;

  /*!
    Returns all the point data
   */
  const_c_array<point>
  point_data(void) const;

  /*!
    Returns the number of outlines
   */
  unsigned int
  number_outlines(void) const;

  /*!
    Returns the range into point_data()
    for the named outline. The outline data is a
    sequence of lines. Points that are shared
    between edges are replicated (because the
    derivatives are different).
   */
  range_type<unsigned int>
  outline_range(unsigned int outline) const;

  /*!
    Returns the range into point_data()
    for the named outline lacking the closing
    edge. The outline data is a sequence of
    lines. Points that are shared between
    edges are replicated (because the
    derivatives are different).
   */
  range_type<unsigned int>
  unclosed_outline_range(unsigned int outline) const;

  /*!
    Returns the point data of the named outline.
    The outline data is a sequence of lines.
   */
  const_c_array<point>
  outline_point_data(unsigned int outline) const;

  /*!
    Returns the point data of the named outline
    lacking the point data of the closing edge.
    The outline data is a sequence of lines.
   */
  const_c_array<point>
  unclosed_outline_point_data(unsigned int outline) const;

  /*!
    Returns the number of edges for the named outline
   */
  unsigned int
  number_edges(unsigned int outline) const;

  /*!
    Returns the range into point_data(void)
    for the named edge of the named outline.
    The returned range does include the end
    point of the edge.
   */
  range_type<unsigned int>
  edge_range(unsigned int outline, unsigned int edge) const;

  /*!
    Returns the point data of the named edge of the
    named outline.
   */
  const_c_array<point>
  edge_point_data(unsigned int outline, unsigned int edge) const;

  /*!
    Returns the minimum point of the bounding box of
    the tessellation.
   */
  vec2
  bounding_box_min(void) const;

  /*!
    Returns the maximum point of the bounding box of
    the tessellation.
   */
  vec2
  bounding_box_max(void) const;

  /*!
    Returns the dimensions of the bounding box
    of the tessellated path.
   */
  vec2
  bounding_box_size(void) const;

  /*!
    Returns this TessellatedPath stroked. Object
    is constructed lazily.
   */
  const StrokedPath::const_handle&
  stroked(void) const;

  /*!
    Returns this TessellatedPath filled. Object
    is constructed lazily.
   */
  const FilledPath::const_handle&
  filled(void) const;

private:
  void *m_d;
};

/*! @} */

}
