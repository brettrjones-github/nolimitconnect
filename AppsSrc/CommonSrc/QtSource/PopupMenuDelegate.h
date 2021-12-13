
#include <QPainter>
#include <QAbstractItemDelegate>

class PopupMenuDelegate : public QAbstractItemDelegate
{
public:
	//=== vars ===//

	//=== constructor ===//
	PopupMenuDelegate(QObject *parent = 0);
	//=== destructor ===//
	virtual ~PopupMenuDelegate();

	//=== overrides ===//
	//! custom paint
	void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	//! size of each item
	QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

};