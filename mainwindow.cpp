#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QMessageBox>
#include <QBrush>
#include <QPen>
#include <QToolButton>
#include <QDebug>
#include "QFileDialog"
#include <QSvgGenerator>
#include <QTableWidget>
#include <QTimeLine>
#include <QProgressBar>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QtMath>
#include <QScrollBar>

#include "workplace.h"
#include "rectangle.h"
#include "line.h"
#include "svssave.h"
#include "graphicsview.h"


MainWindow::MainWindow( QWidget* parent ) :
	QMainWindow( parent ),
	m_ui( new Ui::MainWindow )
{
	m_ui->setupUi( this );

	styleSheets();

	setColor( QColor( Qt::yellow ) );
	setBorderWidth( 2 );
	setLineColor( QColor( Qt::red ) );
	setLineWidth( 1 );
	setBorderColor( Qt::black );

	workplaceScene = new WorkPlace( this );
    workplaceScene->setSceneRect( 0, 0, 1200, 620 );

	workplaceScene_2 = new WorkPlace( this );
	workplaceScene_2->setSceneRect( 0, 0, 1200, 620 );

	workplaceScene_3 = new WorkPlace( this );
    workplaceScene_3->setSceneRect( 0, 0, 1200, 620 );
    this->resize( 1200, 620 );
    this->setFixedSize( 1200, 620 );
	setStyleSheet( "QMenu::item:selected {border: 1px solid black;}" );

	m_ui->graphicsView->setScene( workplaceScene );

	m_ui->graphicsView_3->setScene( workplaceScene_2 );

    scene = new QGraphicsScene( this );
    scene->setItemIndexMethod( QGraphicsScene::NoIndex );
	connects();

	m_ui->widget_line->hide();
	m_ui->widget_rect->hide();
}


MainWindow::~MainWindow()
{
	delete m_ui;
	delete currentRectangle;
	delete currentLine;
}


QColor MainWindow::color() const
{
	return m_color;
}

int MainWindow::borderWidth() const
{
	return m_borderWidth;
}

QColor MainWindow::lineColor() const
{
	return m_lineColor;
}

int MainWindow::lineWidth() const
{
	return m_lineWidth;
}

void MainWindow::connects()
{
	connect( m_ui->lineColor, &ColorLabel::clicked,
			 [ = ]()
	{
		QColorDialog dialog;
		connect( &dialog, &QColorDialog::colorSelected, this, &MainWindow::setColor );
		dialog.exec();
	} );

	connect( m_ui->borderColor, &ColorLabel::clicked,
			 [ = ]()
	{
		QColorDialog dialog;
		connect( &dialog, &QColorDialog::colorSelected, this, &MainWindow::setBorderColor );
		dialog.exec();
	} );

	connect( m_ui->borderColor_3, &ColorLabel::clicked,
			 [ = ]()
	{
		QColorDialog dialog;
		connect( &dialog, &QColorDialog::colorSelected, this, &MainWindow::setLineColor );
		dialog.exec();
	} );


	connect( m_ui->borderWidth, static_cast<void ( QSpinBox::* )( int )>( &QSpinBox::valueChanged ),
			 this, &MainWindow::setBorderWidth );

	connect( m_ui->borderWidth_2, static_cast<void ( QSpinBox::* )( int )>( &QSpinBox::valueChanged ),
			 this, &MainWindow::setLineWidth );


	connect( m_ui->m_square, &QToolButton::clicked, [ = ]()
	{
		workplaceScene->setCurrentAction( WorkPlace::RectangleType );
	} );
	connect( m_ui->m_move, &QToolButton::clicked, [ = ]()
	{
		workplaceScene->setCurrentAction( WorkPlace::DefaultType );
	} );
	connect( m_ui->m_line, &QToolButton::clicked, [ = ]()
	{
		workplaceScene->setCurrentAction( WorkPlace::LineType );
	} );

	connect( workplaceScene, &WorkPlace::signalSelectItem, this, &MainWindow::selectItem );

	connect( workplaceScene, &WorkPlace::signalNewSelectItem, this, &MainWindow::selectNewItem );

	connect( workplaceScene, &WorkPlace::selectionChanged, this, &MainWindow::checkSelections );


	connect( m_ui->m_square, &QToolButton::clicked, [ = ]()
	{
		workplaceScene_2->setCurrentAction( WorkPlace::RectangleType );
	} );
	connect( m_ui->m_move, &QToolButton::clicked, [ = ]()
	{
		workplaceScene_2->setCurrentAction( WorkPlace::DefaultType );
	} );
	connect( m_ui->m_line, &QToolButton::clicked, [ = ]()
	{
		workplaceScene_2->setCurrentAction( WorkPlace::LineType );
	} );

	connect( workplaceScene_2, &WorkPlace::signalSelectItem, this, &MainWindow::selectItem );

	connect( workplaceScene_2, &WorkPlace::signalNewSelectItem, this, &MainWindow::selectNewItem );

	connect( workplaceScene_2, &WorkPlace::selectionChanged, this, &MainWindow::checkSelections );


	connect( m_ui->m_square, &QToolButton::clicked, [ = ]()
	{
		workplaceScene_3->setCurrentAction( WorkPlace::RectangleType );
	} );
	connect( m_ui->m_move, &QToolButton::clicked, [ = ]()
	{
		workplaceScene_3->setCurrentAction( WorkPlace::DefaultType );
	} );
	connect( m_ui->m_line, &QToolButton::clicked, [ = ]()
	{
		workplaceScene_3->setCurrentAction( WorkPlace::LineType );
	} );

	connect( workplaceScene_3, &WorkPlace::signalSelectItem, this, &MainWindow::selectItem );

	connect( workplaceScene_3, &WorkPlace::signalNewSelectItem, this, &MainWindow::selectNewItem );

	connect( workplaceScene_3, &WorkPlace::selectionChanged, this, &MainWindow::checkSelections );


}

void MainWindow::styleSheets()
{

	setWindowIcon( QIcon( ":/icon/Resources/cube.svg" ) );

	QPixmap bkgnd( ":/icon/Resources/grid-background1.jpg" );
	bkgnd = bkgnd.scaled( this->size(), Qt::IgnoreAspectRatio );
	QPalette palette;
	palette.setBrush( QPalette::Background, bkgnd );
	this->setPalette( palette );

	QPixmap pixmapReset( ":/icon/Resources/square.svg" );
	QIcon ButtonIconReset( pixmapReset );
	m_ui->m_square->setIcon( ButtonIconReset );
    m_ui->m_square->setIconSize( QSize( 30, 30 ) );
    m_ui->m_square->setStyleSheet( "background-color:#571835;color:white;" );

	QPixmap pixmapSearch( ":/icon/Resources/pen.svg" );
	QIcon ButtonIconSearch( pixmapSearch );
	m_ui->m_line->setIcon( ButtonIconSearch );
	m_ui->m_line->setIconSize( QSize( 30, 30 ) );
	m_ui->m_line->setStyleSheet( "background-color:#571835;color:white;" );

	QPixmap pixmapClose( ":/icon/Resources/vector.svg" );
	QIcon ButtonIconClose( pixmapClose );
	m_ui->m_move->setIcon( ButtonIconClose );
	m_ui->m_move->setIconSize( QSize( 30, 30 ) );
	m_ui->m_move->setStyleSheet( "background-color:#571835;color:white;");

	m_ui->label->setStyleSheet( "color:white;" );
	m_ui->label_2->setStyleSheet( "color:white;" );
	m_ui->label_3->setStyleSheet( "color:white;" );
	m_ui->label_4->setStyleSheet( "color:white;" );
	m_ui->label_5->setStyleSheet( "color:white;" );

	QFont font( "Courier New" );
	font.setStyleHint( QFont::Monospace );
	font.setPixelSize( 14 );
	QApplication::setFont( font );

}


void MainWindow::setLineColor( const QColor& color )
{
	m_lineColor = color;
	m_ui->borderColor_3->setColor( m_lineColor );

	if ( currentLine != nullptr )
	{
		QPen pen( color, currentLine->pen().width() );
		currentLine->setPen( pen );
	}

	emit lineColorChanged( m_lineColor );
}

void MainWindow::setLineWidth( const int& width )
{
	m_lineWidth = width;
	m_ui->borderWidth_2->setValue( m_lineWidth );

	if ( currentLine != nullptr )
	{
		QPen pen( currentLine->pen().color(), width );
		currentLine->setPen( pen );
	}

	emit lineWidthChanged( m_lineWidth );
}

void MainWindow::setColor( const QColor& color )
{
	m_color = color;
	m_ui->lineColor->setColor( color );

	if ( currentRectangle != nullptr )
	{
		currentRectangle->setBrush( QBrush( m_color ) );
	}

	qDebug() << "color =" << " " << color;
	emit colorChanged( m_color );
}


void MainWindow::setBorderWidth( const int& width )
{
	m_borderWidth = width;

	if ( currentRectangle != nullptr )
	{
		if ( width == 0 )
		{
			currentRectangle->setPen( Qt::NoPen );
		}

		else
		{
			QPen pen( currentRectangle->pen().color(), width );
			currentRectangle->setPen( pen );
		}
	}

	emit borderWidthChanged( m_borderWidth );
}

void MainWindow::setBorderColor( const QColor& color )
{
	m_borderColor = color;
	m_ui->borderColor->setColor( color );

	if ( currentRectangle != nullptr )
	{
		QPen pen( color, currentRectangle->pen().width() );
		currentRectangle->setPen( pen );
	}

	emit borderColorChanged( m_borderColor );
}

void MainWindow::newRectangle( Rectangle* rect )
{
	rect->setBrush( QBrush( m_color ) );

	if ( m_borderWidth == 0 )
	{
		rect->setPen( Qt::NoPen );
	}

	else
	{
		rect->setPen( QPen( m_borderColor, m_borderWidth ) );
	}

}

void MainWindow::loadRectangle( Rectangle* rect )
{
	currentRectangle = rect;

	m_color = currentRectangle->brush().color();

	if ( currentRectangle->pen().style() == Qt::NoPen )
	{
		m_borderWidth = 0;
	}

	else
	{
		m_borderWidth = currentRectangle->pen().width();
	}
}

void MainWindow::newLine( Line* line )
{
	line->setBrush( QBrush( Qt::transparent ) );
	line->setPen( QPen( m_lineColor, m_lineWidth ) );
}

void MainWindow::loadLine( Line* line )
{
	currentLine = line;
	m_lineColor = currentLine->pen().color();
	m_lineWidth = currentLine->pen().width();
	m_ui->borderColor_3->setColor( m_lineColor );
	m_ui->borderWidth_2->setValue( m_lineWidth );
}


void MainWindow::deselect()
{
	currentLine = nullptr;
	currentRectangle = nullptr;
}

void MainWindow::setVisible( bool visible )
{
	if ( !visible && currentRectangle != nullptr )
	{
		QWidget::setVisible( true );
	}

	else
	{
		QWidget::setVisible( visible );
	}

	if ( !visible && currentLine != nullptr )
	{
		QWidget::setVisible( true );
	}

	else
	{
		QWidget::setVisible( visible );
	}
}



void MainWindow::selectNewItem( QGraphicsItem* item )
{
	switch ( item->type() )
	{
		case QGraphicsRectItem::Type:
			{
				Rectangle* rect = qgraphicsitem_cast<Rectangle*>( item );
				newRectangle( rect );
				break;
			}

		case QGraphicsPathItem::Type:
			{
				Line* line = qgraphicsitem_cast<Line*>( item );
				newLine( line );
				break;
			}

		default:
			break;
	}
}

void MainWindow::selectItem( QGraphicsItem* item )
{
	switch ( item->type() )
	{
		case QGraphicsRectItem::Type:
			{
				Rectangle* rect = qgraphicsitem_cast<Rectangle*>( item );
				loadRectangle( rect );
				break;
			}

		case QGraphicsPathItem::Type:
			{
				Line* line = qgraphicsitem_cast<Line*>( item );
				loadLine( line );
				break;
			}

		default:
			break;
	}
}

void MainWindow::checkSelections()
{
	if ( workplaceScene->selectedItems().length() > 1
		 && workplaceScene_2->selectedItems().length() > 1
		 && workplaceScene_3->selectedItems().length() > 1 )
	{
		m_ui->widget_line->setEnabled( false );
		m_ui->widget_rect->setEnabled( false );
	}

	else
	{
		m_ui->widget_line->setEnabled( true );
		m_ui->widget_rect->setEnabled( true );
    }
}

//void MainWindow::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "event " << event->delta();
//    if( event->modifiers() & Qt::ControlModifier )
//    {
//        if ( m_index == 0 )
//        {
//            if( event->modifiers() & Qt::ControlModifier )
//            {
//                m_ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
//                double scaleFactor = 1.15;

//                if( event->delta() > 0 )
//                     m_ui->graphicsView->scale(scaleFactor,scaleFactor);
//                else
//                    m_ui->graphicsView->scale(1/scaleFactor,1/scaleFactor);
//            }
//        }
//        if ( m_index == 1 )
//        {
//            if( event->modifiers() & Qt::ControlModifier )
//            {
//                m_ui->graphicsView_3->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//                double scaleFactor = 1.15;

//                if( event->delta() > 0 )
//                    m_ui->graphicsView_3->scale(scaleFactor,scaleFactor);
//                else
//                    m_ui->graphicsView_3->scale(1/scaleFactor,1/scaleFactor);
//            }
//        }
//        if ( m_index == 2 )
//        {
//            if( event->modifiers() & Qt::ControlModifier )
//            {
//                m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//                double scaleFactor = 1.15;

//                if( event->delta() > 0 )
//                    m_view->scale(scaleFactor,scaleFactor);
//                else
//                    m_view->scale(1/scaleFactor,1/scaleFactor);
//            }
//        }
//    }
//}

void MainWindow::on_m_line_clicked()
{
	m_ui->widget_rect->hide();
	m_ui->widget_line->show();
}

void MainWindow::on_m_square_clicked()
{
	m_ui->widget_line->hide();
	m_ui->widget_rect->show();
}

void MainWindow::on_actionOpen_triggered()
{
	QString newPath = QFileDialog::getOpenFileName( this, trUtf8( "Open SVG" ),
													path, tr( "SVG files (*.svg)" ) );

	if ( newPath.isEmpty() )
	{
		return;
	}

	path = newPath;

	if ( m_index == 0 )
	{
		workplaceScene->clear();
		workplaceScene->setSceneRect( SvgSave::getSizes( path ) );
	}

	if ( m_index == 1 )
	{
		workplaceScene_2->clear();
		workplaceScene_2->setSceneRect( SvgSave::getSizes( path ) );
	}

	if ( m_index == 2 )
	{
		workplaceScene_3->clear();
		workplaceScene_3->setSceneRect( SvgSave::getSizes( path ) );
	}


	foreach ( QGraphicsItem* item, SvgSave::getElements( path ) )
	{
		switch ( item->type() )
		{
			case QGraphicsPathItem::Type:
				{
					Line* line = qgraphicsitem_cast<Line*>( item );

					if ( m_index == 0 )
					{
						workplaceScene->addItem( line );
					}

					if ( m_index == 1 )
					{
						workplaceScene_2->addItem( line );
					}

					if ( m_index == 2 )
					{
						workplaceScene_3->addItem( line );
					}

					break;
				}

			case QGraphicsRectItem::Type:
				{
					Rectangle* rect = qgraphicsitem_cast<Rectangle*>( item );

					if ( m_index == 0 )
					{
						workplaceScene->addItem( rect );
					}

					if ( m_index == 1 )
					{
						workplaceScene_2->addItem( rect );
					}

					if ( m_index == 2 )
					{
						workplaceScene_3->addItem( rect );
					}

					break;
				}

			default:
				break;
		}
	}
}

void MainWindow::on_actionSave_triggered()
{
	QString newPath = QFileDialog::getSaveFileName( this, trUtf8( "Save SVG" ),
													path, tr( "SVG files (*.svg)" ) );

	if ( newPath.isEmpty() )
	{
		return;
	}

	path = newPath;

	QSvgGenerator generator;
	generator.setFileName( path );
	generator.setSize( QSize( workplaceScene->width(), workplaceScene->height() ) );
	generator.setViewBox( QRect( 0, 0, workplaceScene->width(), workplaceScene->height() ) );
	generator.setTitle( trUtf8( "Vector Editor" ) );
	generator.setDescription( trUtf8( "File created by Vector Editor." ) );

	QPainter painter;
	painter.begin( &generator );
	workplaceScene->render( &painter );
	painter.end();
}


void MainWindow::on_tabWidget_tabBarClicked( int index )
{
	qDebug() << index;
	m_index = index;
}

void MainWindow::on_tabWidget_tabCloseRequested( int index )
{
	if ( index > 1 )
	{
		m_ui->tabWidget->removeTab( index );
		m_check = true;
	}
}

void MainWindow::on_actionAdd_tad_triggered()
{
	if ( m_check == true )
	{
        m_view = new QGraphicsView( scene );
        m_view->setScene( workplaceScene_3 );
        m_ui->tabWidget->addTab( m_view, "Subsidiary" );
        m_check = false;
	}

	else
	{
        QMessageBox msgBox(QMessageBox::Warning,
                           "This is the title",
                           "The maximum number of tabs is 3!",
                           QMessageBox::Ok, this,
                           Qt::FramelessWindowHint);
        msgBox.setStyleSheet("QMessageBox { border: none;font-family: Arial; font-style: normal;  font-size: 12pt;};");
		msgBox.exec();
	}

}

void MainWindow::on_actionInfo_triggered()
{
	QMessageBox msgBox(QMessageBox::Question,
					   "This is the title",
					   "Do you love your mother?",
					   QMessageBox::Yes, this,
					   Qt::FramelessWindowHint);
    msgBox.setStyleSheet("QMessageBox { background-image: url(:/icon/Resources/Help.jpg);font-size: 12pt;}");
	msgBox.exec();
}

//void MainWindow::on_horizontalSlider_valueChanged(int value)
//{
//    qDebug () << value;
//    m_ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//    double scaleFactor = 1.04;

//    if( value > 0 )
//         m_ui->graphicsView->scale(scaleFactor,scaleFactor);
//    else
//    {
//        m_ui->graphicsView->scale(1/scaleFactor,1/scaleFactor);
//    }

//}
