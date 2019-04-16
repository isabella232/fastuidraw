/*!
 * \file painter_backend_factory.hpp
 * \brief file painter_backend_factory.hpp
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

#include <fastuidraw/util/blend_mode.hpp>
#include <fastuidraw/util/rect.hpp>
#include <fastuidraw/text/glyph_atlas.hpp>
#include <fastuidraw/image.hpp>
#include <fastuidraw/colorstop_atlas.hpp>
#include <fastuidraw/painter/backend/painter_backend.hpp>


namespace fastuidraw
{
/*!\addtogroup PainterBackend
 * @{
 */

  /*!
   * \brief
   * A \ref PainterBackendFactory provides an interface to create
   * \ref PainterBackend derived objects.
   */
  class PainterBackendFactory:public reference_counted<PainterBackendFactory>::concurrent
  {
  public:
    /*!
     * To be implemented by a derived class to create a
     * \ref PainterBackend object. All \ref PainterBackend
     * objects created by create_backend() from the same
     * \ref PainterBackendFactory share the same
     *  - \ref PainterShaderRegistrar (see \ref PainterBackend::painter_shader_registrar())
     *  - \ref GlyphAtlas (see \ref PainterBackend::glyph_atlas())
     *  - \ref ImageAtlas (see \ref PainterBackend::image_atlas())
     *  - \ref ColorStopAtlas (see \ref PainterBackend::colorstop_atlas())
     * but are otherwise independent of each other.
     */
    virtual
    reference_counted_ptr<PainterBackend>
    create_backend(void) const = 0;
  };
/*! @} */

}
