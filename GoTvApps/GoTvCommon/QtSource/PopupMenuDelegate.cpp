
#include <app_precompiled_hdr.h>
#include "GuiParams.h"

#include <QPainter>
#include <QAbstractItemDelegate>

#include "PopupMenuDelegate.h"

PopupMenuDelegate::PopupMenuDelegate(QObject *parent)
{
}
PopupMenuDelegate::~PopupMenuDelegate()
{
}

void PopupMenuDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QRect r = option.rect;
	//Color: #C4C4C4
	QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);
	//Color: #005A83
	QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);
	//Color: #333
	QPen fontPen(QColor::fromRgb(255,255,255), 1, Qt::SolidLine);
	//Color: #fff
	QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);
	if(option.state & QStyle::State_Selected)
	{
		QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
		//gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
		//gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
		//gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
		gradientSelected.setColorAt(0.0, QColor::fromRgb(119,247,213));
		gradientSelected.setColorAt(0.9, QColor::fromRgb(2,1837,134));
		gradientSelected.setColorAt(1.0, QColor::fromRgb(0,174,120));
		painter->setBrush(gradientSelected);
		painter->drawRect(r);
		//BORDER
		painter->setPen(lineMarkedPen);
		painter->drawLine(r.topLeft(),r.topRight());
		painter->drawLine(r.topRight(),r.bottomRight());
		painter->drawLine(r.bottomLeft(),r.bottomRight());
		painter->drawLine(r.topLeft(),r.bottomLeft());
		painter->setPen(fontMarkedPen);
	} 
	else 
	{
		//BACKGROUND
		//ALTERNATING COLORS
		painter->setBrush( (index.row() % 2) ? Qt::black : QColor(8,8,8) );
		painter->drawRect(r);
		//BORDER
		painter->setPen(linePen);
		painter->drawLine(r.topLeft(),r.topRight());
		painter->drawLine(r.topRight(),r.bottomRight());
		painter->drawLine(r.bottomLeft(),r.bottomRight());
		painter->drawLine(r.topLeft(),r.bottomLeft());

		painter->setPen(fontPen);
	}
	//GET TITLE, DESCRIPTION AND ICON
	//QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
	QIcon ic = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
	QString title = index.data(Qt::DisplayRole).toString();

	int imageSpace = 10;
	if (!ic.isNull()) 
	{
		//ICON
		r = option.rect.adjusted(5, 5, -5, -5);
		ic.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);
		imageSpace = 55;
	}

	//TITLE
	r = option.rect.adjusted(imageSpace, 0, -5, -30);
	painter->setFont( QFont( "Lucida Grande", 12, QFont::Normal ) );
	painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, title, &r);
}

QSize PopupMenuDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	return QSize(200, GuiParams::getButtonSize() + 4); // very dumb value
}

