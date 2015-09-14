/****************************************************************************
** Copyright (C) 2007-2015 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "KDReportsPreviewDialog.h"
#include "KDReportsReport.h"
#include "KDReportsReport_p.h"
#include "KDReportsPreviewWidget.h"
#include "KDReportsTableBreakingSettingsDialog.h"
#include "KDReportsLayoutHelper_p.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

class KDReports::PreviewDialog::Private
{
public:
    Private( KDReports::PreviewDialog* q) : q( q ), m_previewWidget( 0 ) {}

    void _kd_slotTableBreakingDialog();
    void _kd_slotPrintWithDialog();
    void _kd_slotQuickPrint();

    KDReports::PreviewDialog* q;
    KDReports::PreviewWidget* m_previewWidget;
    QDialogButtonBox *m_buttonBox;
    QPushButton *m_quickPrintButton;
    QString m_quickPrinterName;
};

KDReports::PreviewDialog::PreviewDialog( KDReports::Report* report, QWidget *parent )
    : QDialog( parent ), d( new Private( this ) )
{
    d->m_previewWidget = new KDReports::PreviewWidget( this );
    d->m_previewWidget->setReport( report );
    QVBoxLayout* topLayout = new QVBoxLayout( this );
    topLayout->addWidget( d->m_previewWidget );
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    topLayout->addLayout( bottomLayout );

    connect( d->m_previewWidget, SIGNAL(tableSettingsClicked()), this, SLOT(_kd_slotTableBreakingDialog()) );

    d->m_buttonBox = new QDialogButtonBox( Qt::Horizontal, this );
    bottomLayout->addWidget( d->m_buttonBox );

    QPushButton* printWithDialogButton = new QPushButton( tr("&Print..."), this );
    d->m_buttonBox->addButton( printWithDialogButton, QDialogButtonBox::ActionRole );
    connect( printWithDialogButton, SIGNAL(clicked()), this, SLOT(_kd_slotPrintWithDialog()) );

    d->m_quickPrintButton = new QPushButton( this ); // create it here for the ordering
    d->m_quickPrintButton->hide();
    d->m_buttonBox->addButton( d->m_quickPrintButton, QDialogButtonBox::ActionRole );

    QPushButton* cancelButton = new QPushButton( tr("Cancel"), this );
    d->m_buttonBox->addButton( cancelButton, QDialogButtonBox::RejectRole );
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(reject()) );
}

KDReports::PreviewDialog::~PreviewDialog()
{
    delete d;
}

void KDReports::PreviewDialog::setQuickPrinterName( const QString &printerName )
{
    if ( !printerName.isEmpty() ) {
        // QPrinter::setPrinterName has no effect on the QPrintDialog which uses the CUPS default printer anyway...
        // So we only use this for the separate Print button.
        // This is why it's not called setDefaultPrinterName.
        d->m_quickPrinterName = printerName;
        d->m_quickPrintButton->setText( tr("Print &with %1").arg(printerName) );
        d->m_quickPrintButton->show();
        connect( d->m_quickPrintButton, SIGNAL(clicked()), this, SLOT(_kd_slotQuickPrint()) );
    }
}

bool KDReports::PreviewDialog::showTableSettingsDialog( KDReports::Report* report )
{
    KDReports::TableBreakingSettingsDialog dialog( report );
    return dialog.exec();
}

void KDReports::PreviewDialog::Private::_kd_slotTableBreakingDialog()
{
    if ( q->showTableSettingsDialog( m_previewWidget->report() ) ) {
        m_previewWidget->repaint();
    }
}

void KDReports::PreviewDialog::Private::_kd_slotPrintWithDialog()
{
    if ( m_previewWidget->printWithDialog() ) {
        q->accept();
    }
}

void KDReports::PreviewDialog::Private::_kd_slotQuickPrint()
{
    KDReports::Report *report = m_previewWidget->report();
    QPrinter printer;
    printer.setFullPage( true );
    printer.setPrinterName( m_quickPrinterName );
    printer.setPageSize( report->pageSize() );
    printer.setOrientation( report->orientation() );
    report->print( &printer, q );
    q->accept();
}

void KDReports::PreviewDialog::setPageSizeChangeAllowed( bool b )
{
    d->m_previewWidget->setPageSizeChangeAllowed( b );
}

void KDReports::PreviewDialog::setShowTableSettingsDialog( bool b )
{
    d->m_previewWidget->setShowTableSettingsDialog( b );
}

void KDReports::PreviewDialog::setWidthForEndlessPrinter( qreal widthMM )
{
    d->m_previewWidget->setWidthForEndlessPrinter( widthMM );
}

bool KDReports::PreviewDialog::isSelected( int pageNumber ) const
{
    return d->m_previewWidget->isSelected( pageNumber );
}

void KDReports::PreviewDialog::accept()
{
    QDialog::accept();
}

void KDReports::PreviewDialog::reject()
{
    QDialog::reject();
}

KDReports::PreviewWidget * KDReports::PreviewDialog::previewWidget()
{
    return d->m_previewWidget;
}

#include "moc_KDReportsPreviewDialog.cpp"
