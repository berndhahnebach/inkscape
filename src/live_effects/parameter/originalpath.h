#ifndef INKSCAPE_LIVEPATHEFFECT_PARAMETER_ORIGINAL_PATH_H
#define INKSCAPE_LIVEPATHEFFECT_PARAMETER_ORIGINAL_PATH_H

/*
 * Inkscape::LivePathEffectParameters
 *
* Copyright (C) Johan Engelen 2012 <j.b.c.engelen@alumnus.utwente.nl>
 *
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#include "live_effects/parameter/path.h"

namespace Inkscape {

namespace LivePathEffect {

class OriginalPathParam: public PathParam {
public:
    OriginalPathParam ( const Glib::ustring& label,
                const Glib::ustring& tip,
                const Glib::ustring& key,
                Inkscape::UI::Widget::Registry* wr,
                Effect* effect);
    virtual ~OriginalPathParam();

    bool linksToPath() { return (href != NULL); }

    virtual Gtk::Widget * param_newWidget();
    /** Disable the canvas indicators of parent class by overriding this method */
    virtual void param_editOncanvas(SPItem * /*item*/, SPDesktop * /*dt*/) {};
    /** Disable the canvas indicators of parent class by overriding this method */
    virtual void addCanvasIndicators(SPLPEItem * /*lpeitem*/, std::vector<Geom::PathVector> & /*hp_vec*/) {};

protected:
    virtual void linked_modified_callback(SPObject *linked_obj, guint flags);
    void on_select_original_button_click();

private:
    OriginalPathParam(const OriginalPathParam&);
    OriginalPathParam& operator=(const OriginalPathParam&);
};


} //namespace LivePathEffect

} //namespace Inkscape

#endif