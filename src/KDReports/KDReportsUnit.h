/****************************************************************************
** Copyright (C) 2007-2018 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Reports library.
**
** Licensees holding valid commercial KD Reports licenses may use this file in
** accordance with the KD Reports Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU Lesser General Public License version 2.1 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.LGPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDREPORTSUNIT_H
#define KDREPORTSUNIT_H

namespace KDReports
{

/**
 * Units used when setting a size.
 */
enum Unit {
    Millimeters, ///< Millimeters (the default)
    Percent      ///< Percentage of the text width, i.e. the page width minus margins
};

}

#endif /* KDREPORTSUNIT_H */

