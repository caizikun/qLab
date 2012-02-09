#include "tableview.h"

TableView::TableView(QWidget *parent) :
    QTableView(parent)
{

}


void TableView::copy() {
    if (!model()) {
        qWarning()<<"Can not copy, model is empty";
        return;
    }

    QItemSelectionModel * selection = selectionModel();
    if (!selection) {
        qWarning()<<"No selection model";
        return;
    }

    QModelIndexList indexes = selection->selectedIndexes();
    if (indexes.isEmpty()) return;
    qSort(indexes);

    QString text;
    for (int i=0,row=indexes.at(i).row();i<indexes.size();i++) {

        if (row!=indexes.at(i).row()) {
            text+="\n";
            row=indexes.at(i).row();
        } else {
            text+="\t";
        }
        text+=indexes.at(i).data().toString();
    }
    text=text.trimmed();

    QClipboard *clipboard=QApplication::clipboard();
    qDebug()<<"Copied text"<<text;
    clipboard->setText(text);
}


void TableView::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::Copy)) {
        copy();
        return;
    }
    QTableView::keyPressEvent(event);
}
