#include "editor.h"
#include "ui_drawer.h"

Editor::Editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);
    // PaintArea
}

Editor::~Editor()
{
    delete ui;
}

void Editor::Init(PaintArea* painter_)
{
    this->painter = painter_;
}





