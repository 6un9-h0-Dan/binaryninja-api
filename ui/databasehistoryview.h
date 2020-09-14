#pragma once

#include <QWidget>
#include "viewframe.h"
#include "flowgraphwidget.h"

class DatabaseHistoryContainer;

class BINARYNINJAUIAPI DatabaseHistoryFlowGraph: public BinaryNinja::FlowGraph
{
	BinaryViewRef m_data;

	void update();

public:
	DatabaseHistoryFlowGraph(BinaryViewRef data);
};


class BINARYNINJAUIAPI DatabaseHistoryView: public FlowGraphWidget
{
	Q_OBJECT

	BinaryViewRef m_data;
	ViewFrame* m_frame;
	DatabaseHistoryFlowGraph* m_flowGraph;
	QFont m_font;

public:
	DatabaseHistoryView(BinaryViewRef data, ViewFrame* frame, DatabaseHistoryContainer* container);

	virtual BinaryViewRef getData() override { return m_data; }
	virtual uint64_t getCurrentOffset() override { return 0; }
	virtual void setSelectionOffsets(BNAddressRange range) override { }
	virtual bool navigate(uint64_t offset) override { return false; }
	virtual QFont getFont() override { return m_font; }
};


class DatabaseHistoryContainer: public QWidget, public ViewContainer
{
	Q_OBJECT
	ViewFrame* m_frame;
	DatabaseHistoryView* m_view;

public:
	DatabaseHistoryContainer(BinaryViewRef data, ViewFrame* frame);

	virtual View* getView() override { return m_view; }

};


class BINARYNINJAUIAPI DatabaseHistoryViewType: public ViewType
{
	static DatabaseHistoryViewType* m_instance;

public:
	DatabaseHistoryViewType();
	virtual int getPriority(BinaryViewRef data, const QString& filename) override;
	virtual QWidget* create(BinaryViewRef data, ViewFrame* viewFrame) override;
	static void init();
};

