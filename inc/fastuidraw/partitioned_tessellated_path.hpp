/*!
 * \file partitioned_tessellated_path.hpp
 * \brief file partitioned_tessellated_path.hpp
 *
 * Copyright 2019 by Intel.
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

#include <fastuidraw/util/matrix.hpp>
#include <fastuidraw/util/vecN.hpp>
#include <fastuidraw/util/c_array.hpp>
#include <fastuidraw/util/util.hpp>
#include <fastuidraw/util/rect.hpp>
#include <fastuidraw/tessellated_path.hpp>
#include <fastuidraw/path_enums.hpp>

namespace fastuidraw  {

/*!\addtogroup Paths
 * @{
 */

  /*!
   * \brief
   * A PartitionedTessellatedPath represents partitioning a \ref
   * TessellatedPath for quick computation of what \ref
   * TessellatedPath::segment intersect a region.
   *
   * The partitioning creates reectangular regions of the source
   * \ref TessellatedPath that are disjoint. One side effect of
   * that is that a \ref TessellatedPath::segment value from the
   * source  \ref  TessellatedPath may be split into multiple \ref
   * TessellatedPath::segment values.
   */
  class PartitionedTessellatedPath:
    public reference_counted<PartitionedTessellatedPath>::non_concurrent
  {
  public:
    /*!
     * A \ref segment is the same as found in \ref TessellatedPath
     */
    typedef TessellatedPath::segment segment;

    /*!
     * A \ref join is the same as found in \ref TessellatedPath
     */
    typedef TessellatedPath::join join;

    /*!
     * A \ref cap is the same as found in \ref TessellatedPath
     */
    typedef TessellatedPath::cap cap;

    /*!
     * A \ref segment_chain is a sequence of \ref
     * segment values where successive elements are
     * neighbors of the same edge in the source \ref
     * TessellatedPath
     */
    class segment_chain
    {
    public:
      /*!
       * The chain of \ref segment values
       */
      c_array<const segment> m_segments;

      /*!
       * if non-null, gives the segment just
       * before the first element of \ref
       * m_segments. If null, then there is
       * no segment just before \ref m_segments.
       */
      const segment *m_prev_to_start;
    };

    /*!
     * \brief
     * A Subset represents a handle to a portion of a \ref
     * PartitionedTessellatedPath. The handle is invalid once the
     * \ref PartitionedTessellatedPath from which it comes goes
     * out of scope. Do not save these handle values without also
     * saving a handle of the \ref PartitionedTessellatedPath
     * from which they come.
     */
    class Subset
    {
    private:
      typedef bool (Subset::*unspecified_bool_type)(void) const;

    public:
      /*!
       * Ctor to initialize value to "null" handle.
       */
      Subset(void):
        m_d(nullptr)
      {}

      /*!
       * Allows one to legally write to test if Subset
       * is a null-handle:
       * \code
       * Subset p;
       *
       * if (p)
       *   {
       *     // p does refers to data
       *   }
       *
       * if (!p)
       *   {
       *     // p does not referr to any data
       *   }
       * \endcode
       */
      operator unspecified_bool_type() const
      {
        return m_d ? &Subset::has_children : 0;
      }

      /*!
       * Returns the bounding box.
       */
      const Rect&
      bounding_box(void) const;

      /*!
       * Returns the segments that are within this
       * \ref Subset
       */
      c_array<const segment_chain>
      segment_chains(void) const;

      /*!
       * Returns the joins within this \ref Subset
       */
      c_array<const join>
      joins(void) const;

      /*!
       * Returns the caps within this \ref Subset
       */
      c_array<const cap>
      caps(void) const;

      /*!
       * Returns the ID of this Subset, i.e. the value to
       * feed to \ref PartitionedTessellatedPath::subset()
       * to get this \ref Subset.
       */
      unsigned int
      ID(void) const;

      /*!
       * Returns true if this Subset has child Subset
       */
      bool
      has_children(void) const;

      /*!
       * Returns the children of this Subset. It is an
       * error to call this if \ref has_children() returns
       * false.
       */
      vecN<Subset, 2>
      children(void) const;

    private:
      friend class PartitionedTessellatedPath;

      Subset(void *d);

      void *m_d;
    };

    /*!
     * A SubsetSelection represents what \ref Subset
     * objects intersect a clipped region.
     */
    class SubsetSelection:fastuidraw::noncopyable
    {
    public:
      /*!
       * Ctor.
       */
      SubsetSelection();
      ~SubsetSelection();

      /*!
       * The ID's of what Subset objects are selected.
       */
      c_array<const unsigned int>
      subset_ids(void) const;

      /*!
       * ID's of what Subset objects are selected for joins.
       * This value is different from \ref subset_ids() only
       * when PartitionedTessellatedPath::select_subset_ids()
       * was specified to enlarge the join footprints for
       * miter-join stroking.
       */
      c_array<const unsigned int>
      join_subset_ids(void) const;

      /*!
       * Returns the source for the data.
       */
      const reference_counted_ptr<const PartitionedTessellatedPath>&
      source(void) const;

      /*!
       * Clears the SubsetSelection to be empty.
       * \param src value for \ref source() to return
       */
      void
      clear(const reference_counted_ptr<const PartitionedTessellatedPath> &src =
            reference_counted_ptr<const PartitionedTessellatedPath>());

    private:
      friend class PartitionedTessellatedPath;

      void *m_d;
    };

    ~PartitionedTessellatedPath();

    /*!
     * Returns true if any of the \ref segment values of any
     * of the \ref segment_chain values of any of the \ref
     * Subset::segment_chains() has \ref segment::m_type equal
     * to \ref TessellatedPath::arc_segment.
     */
    bool
    has_arcs(void) const;

    /*!
     * Returns the maximum across all edges of all contours
     * of the distance between the tessellation and the actual
     * path.
     */
    float
    max_distance(void) const;

    /*!
     * Returns the number of Subset objects of the StrokedPath.
     */
    unsigned int
    number_subsets(void) const;

    /*!
     * Return the named Subset object of the StrokedPath.
     */
    Subset
    subset(unsigned int I) const;

    /*!
     * Returns the joins of the path.
     */
    c_array<const join>
    joins(void) const;

    /*!
     * Returns the caps of the path.
     */
    c_array<const cap>
    caps(void) const;

    /*!
     * Returns the root-subset of the StrokedPath, this
     * is the \ref Subset that includes the entire
     * StrokedPath.
     */
    Subset
    root_subset(void) const;

    /*!
     * Given a set of clip equations in clip coordinates
     * and a tranformation from local coordiante to clip
     * coordinates, compute what Subset are not completely
     * culled by the clip equations.
     * \param clip_equations array of clip equations
     * \param clip_matrix_local 3x3 transformation from local (x, y, 1)
     *                          coordinates to clip coordinates.
     * \param one_pixel_width holds the size of a single pixel in
     *                        normalized device coordinates
     * \param geometry_inflation amount path geometry is inflated, array
     *                           is indexed by the enumeration \ref
     *                           PathEnums::path_geometry_inflation_index_t
     * \param select_miter_joins if true, when selecting what joins are in
     *                           the area, enlarge the join footprint for if
     *                           the joins are stroked as a type of miter join.
     * \param[out] dst location to which to write the subset-selection.
     */
    void
    select_subsets(c_array<const vec3> clip_equations,
                   const float3x3 &clip_matrix_local,
                   const vec2 &one_pixel_width,
                   c_array<const float> geometry_inflation,
                   bool select_miter_joins,
                   SubsetSelection &dst) const;
  private:
    friend class TessellatedPath;

    // Only TessellatedPath can construct a PartitionedTessellatedPath
    explicit
    PartitionedTessellatedPath(const TessellatedPath &path);

    void *m_d;
  };

/*! @} */
}
