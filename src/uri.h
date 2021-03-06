/*
 * Authors:
 *   MenTaLguY <mental@rydia.net>
 *   Jon A. Cruz <jon@joncruz.org>
 *
 * Copyright (C) 2003 MenTaLguY
 *
 * Released under GNU GPL, read the file 'COPYING' for more information
 */

#ifndef INKSCAPE_URI_H
#define INKSCAPE_URI_H

#include <glib.h>
#include <exception>
#include <libxml/uri.h>
#include "bad-uri-exception.h"

namespace Inkscape {

/**
 * Represents an URI as per RFC 2396.
 */
class URI {
public:

    /**
     * Copy constructor.
     */
    URI(URI const &uri);

    /**
     * Constructor from a C-style ASCII string.
     *
     * @param preformed Properly quoted C-style string to be represented.
     */
    explicit URI(gchar const *preformed) throw(BadURIException);

    /**
     * Destructor.
     */
    ~URI();

    /**
     * Determines if the URI represented is an 'opaque' URI.
     *
     * @return \c true if the URI is opaque, \c false if hierarchial.
     */
    bool isOpaque() const { return _impl->isOpaque(); }

    /**
     * Determines if the URI represented is 'relative' as per RFC 2396.
     *
     * Relative URI references are distinguished by not begining with a
     * scheme name.
     *
     * @return \c true if the URI is relative, \c false if it is absolute.
     */
    bool isRelative() const { return _impl->isRelative(); }

    /**
     * Determines if the relative URI represented is a 'net-path' as per RFC 2396.
     *
     * A net-path is one that starts with "\\".
     *
     * @return \c true if the URI is relative and a net-path, \c false otherwise.
     */
    bool isNetPath() const { return _impl->isNetPath(); }

    /**
     * Determines if the relative URI represented is a 'relative-path' as per RFC 2396.
     *
     * A relative-path is one that starts with no slashes.
     *
     * @return \c true if the URI is relative and a relative-path, \c false otherwise.
     */
    bool isRelativePath() const { return _impl->isRelativePath(); }

    /**
     * Determines if the relative URI represented is a 'absolute-path' as per RFC 2396.
     *
     * An absolute-path is one that starts with a single "\".
     *
     * @return \c true if the URI is relative and an absolute-path, \c false otherwise.
     */
    bool isAbsolutePath() const { return _impl->isAbsolutePath(); }

    const gchar *getScheme() const { return _impl->getScheme(); }

    const gchar *getPath() const { return _impl->getPath(); }

    const gchar *getQuery() const { return _impl->getQuery(); }

    const gchar *getFragment() const { return _impl->getFragment(); }

    const gchar *getOpaque() const { return _impl->getOpaque(); }

    static URI fromUtf8( gchar const* path ) throw (BadURIException);

    static URI from_native_filename(gchar const *path) throw(BadURIException);

    static gchar *to_native_filename(gchar const* uri) throw(BadURIException);

    gchar *toNativeFilename() const throw(BadURIException);

    /**
     * Returns a glib string version of this URI.
     *
     * The returned string must be freed with \c g_free().
     *
     * @return a glib string version of this URI.
     */
    gchar *toString() const { return _impl->toString(); }

    /**
     * Assignment operator.
     */
    URI &operator=(URI const &uri);

private:
    class Impl {
    public:
        static Impl *create(xmlURIPtr uri);
        void reference();
        void unreference();

        bool isOpaque() const;
        bool isRelative() const;
        bool isNetPath() const;
        bool isRelativePath() const;
        bool isAbsolutePath() const;
        const gchar *getScheme() const;
        const gchar *getPath() const;
        const gchar *getQuery() const;
        const gchar *getFragment() const;
        const gchar *getOpaque() const;
        gchar *toString() const;
    private:
        Impl(xmlURIPtr uri);
        ~Impl();
        int _refcount;
        xmlURIPtr _uri;
    };
    Impl *_impl;
};

}  /* namespace Inkscape */

#endif

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
