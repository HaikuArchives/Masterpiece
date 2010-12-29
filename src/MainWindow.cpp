
/*
 *
 * Error Codes
 * 1.x - Sql Errors
 * 2.x - New MP
 * 3.x - Open MP
 *
 */

// 1.  Need to check and see if an mp is already open, prior to creating a new one,
// 	   so i can reload what was there and what view was there...

#include "MainWindow.h"

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,900,700),"MasterPiece",B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	mainGrid = new BGridLayout();
	mainCard = new BCardLayout();
	mpMenuBar = new MPMenuBar(Bounds());
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	backView->SetLayout(mainGrid);
	mainGrid->AddView(mpMenuBar, 0, 0, 1, 2);
	mainGrid->AddItem(mainCard, 0, 2, 1, 1);
	
	BRect placeHolderRect(Bounds());

	placeHolderView = new PlaceHolderView(placeHolderRect);
	mainCard->AddView(placeHolderView); // item 0

	sumView = new SummaryView(placeHolderRect);
	mainCard->AddView(sumView); // item 1
	
	thoughtView = new ThoughtView(placeHolderRect);
	mainCard->AddView(thoughtView); // item 2
	
	mainCard->SetVisibleItem((long)0);
}

void MainWindow::Draw(BRect rect)
{
}

void MainWindow::MessageReceived(BMessage* msg)
{
	BRect r(Bounds());	
	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// 1.  need to center the modal window on the parent...
			// 2.  check to see if course is currently open
			xPos = (r.right - r.left) / 2;
			yPos = (r.bottom - r.top) / 2;
			newWin = new NewWindow(BMessage(UPDATE_NEW_MP), BMessenger(this), xPos, yPos);
			newWin->Show();
			break;
		
		case UPDATE_NEW_MP:
			if(msg->FindString("mptitle", &mptitle) == B_OK && msg->FindInt64("mpid", &mpid) == B_OK)
			{
				this->SetTitle(mptitle);
				// 1.  need to figure out how to open the summaryview using the mpid
				tmpString = mptitle;
				tmpString += " Summary";
				this->sumView->sumViewTitleString->SetText(tmpString);
				this->mainCard->SetVisibleItem((long)1);
				this->mpMenuBar->contentMenu->SetEnabled(true);
				this->mpMenuBar->layoutMenu->SetEnabled(true);
				this->mpMenuBar->closeFileMenuItem->SetEnabled(true);
				// 1.  call the function to populate summaryview passing the (mpid)
			}
			break;
		
		case MENU_OPN_MSG:
			xPos = (r.right - r.left) / 2;
			yPos = (r.bottom - r.top) / 2;
			openWin = new OpenWindow(BMessage(UPDATE_OPEN_MP), BMessenger(this), xPos, yPos, "", mpdb);
			openWin->Show();
			break;
		
		case UPDATE_OPEN_MP:
			if(msg->FindString("opentitle", &mptitle) == B_OK && msg->FindInt64("openid", &mpid) == B_OK)
			{
				this->SetTitle(mptitle);
				tmpString = mptitle;
				tmpString += " Summary";
				this->sumView->sumViewTitleString->SetText(tmpString);
				this->mainCard->SetVisibleItem((long)1);
				
				this->mpMenuBar->summaryMenuItem->SetEnabled(false);
				this->mpMenuBar->contentMenu->SetEnabled(true);
				this->mpMenuBar->layoutMenu->SetEnabled(true);
				this->mpMenuBar->closeFileMenuItem->SetEnabled(true);
			}
			
			break;
			
		case MENU_CLS_MSG:
			// 1.  close course - simply clear values and hide views.
			this->mainCard->SetVisibleItem((long)0);
			this->SetTitle("MasterPiece");
			this->mpMenuBar->closeFileMenuItem->SetEnabled(false);
			this->mpMenuBar->contentMenu->SetEnabled(false);
			this->mpMenuBar->layoutMenu->SetEnabled(false);
			
			break;

		case MENU_THT_MSG:
			this->mainCard->SetVisibleItem((long)2);
			this->mpMenuBar->thoughtsMenuItem->SetEnabled(false);
			this->mpMenuBar->summaryMenuItem->SetEnabled(true);
			// when in a view, might want to invalidate the menu option so it can't be redone and screwed up for now
			// do something here...
			break;
		
		case MNG_LAYOUT_MSG:
			// do something here...
			break;
			
		case MENU_SUM_MSG:
			this->mainCard->SetVisibleItem((long)1);
			this->mpMenuBar->summaryMenuItem->SetEnabled(false);
			this->mpMenuBar->thoughtsMenuItem->SetEnabled(true);
			// do something here...
			break;

		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

void MainWindow::FrameResized(float width, float height)
{
}

bool
MainWindow::QuitRequested(void)
{
	sqlite3_free(sqlErrMsg);
	sqlite3_close(mpdb);
	
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void MainWindow::PopulateSummaryView(int mpID)
{
	//tmpString = "select thoughtID, thoughtData from ttable wher
	// place code here to populate summary view based on input id...
}
int MainWindow::OpenMasterPieceDB()
{
	sqlErrMsg = 0;
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	BString tmpPath = path.Path();
	tmpPath += "/MasterPiece.db";
	sqlValue = sqlite3_open_v2(tmpPath, &mpdb, SQLITE_OPEN_READWRITE, NULL); // open masterpiece.db
	if(sqlite3_errcode(mpdb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure.
	{
		sqlValue = sqlite3_open_v2(tmpPath, &mpdb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); // create masterpiece.db
		if(sqlite3_errcode(mpdb) == 0) // sqlite_ok
		{
			tmpString = "create table mptable(mpid integer primary key autoincrement, mpname text);";
			tmpString += " create table ideatable(ideaid integer primary key autoincrement, ideadata blob, ideatype integer, mpid integer, ordernumber integer);";
			tmpString += " create table itypetable(itypeid integer primary key autoincrement, itypename text, itypedescription text);";
			sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
			if(sqlValue == SQLITE_OK) // if sql was successful
			{
				// ladida...
			}
			else // sql not successful
			{
				errorAlert = new ErrorAlert("1.1 SQL Error: ", sqlErrMsg);
				errorAlert->Launch();
			}
		}
		else // some kind of failure...
		{
			errorAlert = new ErrorAlert("1.0 Sql Error: ", sqlite3_errmsg(mpdb));
			errorAlert->Launch();
		}
	}
	else if(sqlite3_errcode(mpdb) == 0) // SQLITE_OK, it exists
	{
		// ladida
	}
	else // if error is not ok or not existing, then display error in alert.
	{
		errorAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(mpdb));
		errorAlert->Launch();
		this->mpMenuBar->fileMenu->SetEnabled(false);
	}
	return 0;
}
/*
#ifndef	_CARD_LAYOUT_H
#define	_CARD_LAYOUT_H

#include <AbstractLayout.h>


class BCardLayout : public BAbstractLayout {
public:
								BCardLayout();
								BCardLayout(BMessage* from);
	virtual						~BCardLayout();

			BLayoutItem*		VisibleItem() const;
			int32				VisibleIndex() const;
			void				SetVisibleItem(int32 index);
			void				SetVisibleItem(BLayoutItem* item);

	virtual	BSize				BaseMinSize();
	virtual	BSize				BaseMaxSize();
	virtual	BSize				BasePreferredSize();
	virtual	BAlignment			BaseAlignment();

	virtual	bool				HasHeightForWidth();
	virtual	void				GetHeightForWidth(float width, float* min,
									float* max, float* preferred);

	virtual	void				InvalidateLayout(bool children = false);

	virtual status_t			Archive(BMessage* into, bool deep = true) const;
	virtual status_t			AllUnarchived(const BMessage* from);
	static	BArchivable*		Instantiate(BMessage* from);

protected:
	virtual	void				DerivedLayoutItems();
	virtual	bool				ItemAdded(BLayoutItem* item, int32 atIndex);
	virtual	void				ItemRemoved(BLayoutItem* item, int32 fromIndex);

private:
			BSize				fMin;
			BSize				fMax;
			BSize				fPreferred;
			BLayoutItem*		fVisibleItem;
			bool				fMinMaxValid;

			void				_ValidateMinMax();
};

#endif	// _CARD_LAYOUT_H
*/

/*
#ifndef	_ABSTRACT_LAYOUT_H
#define	_ABSTRACT_LAYOUT_H

#include <Alignment.h>
#include <Layout.h>
#include <Size.h>

class BAbstractLayout : public BLayout {
public:
								BAbstractLayout();
								BAbstractLayout(BMessage* from);
	virtual						~BAbstractLayout();

	virtual	BSize				MinSize();
	virtual	BSize				MaxSize();
	virtual	BSize				PreferredSize();
	virtual	BAlignment			Alignment();

	virtual	void				SetExplicitMinSize(BSize size);
	virtual	void				SetExplicitMaxSize(BSize size);
	virtual	void				SetExplicitPreferredSize(BSize size);
	virtual	void				SetExplicitAlignment(BAlignment alignment);

	virtual	BSize				BaseMinSize();
	virtual	BSize				BaseMaxSize();
	virtual	BSize				BasePreferredSize();
	virtual	BAlignment			BaseAlignment();

	virtual BRect				Frame();
	virtual	void				SetFrame(BRect frame);	

	virtual	bool				IsVisible();
	virtual	void				SetVisible(bool visible);

	virtual status_t			Archive(BMessage* into, bool deep = true) const;
	virtual	status_t			AllUnarchived(const BMessage* from);

protected:
	virtual	void				OwnerChanged(BView* was);
	virtual	void				AncestorVisibilityChanged(bool shown);

private:
			struct	Proxy;
			struct	ViewProxy;
			struct	DataProxy;

			Proxy*				fExplicitData;
};

#endif	//	_ABSTRACT_LAYOUT_ITEM_H

*/

/*
#ifndef	_LAYOUT_H
#define	_LAYOUT_H


#include <Alignment.h>
#include <Archivable.h>
#include <LayoutItem.h>
#include <List.h>
#include <Size.h>


class BLayoutContext;
class BLayoutItem;
class BView;


class BLayout : public BLayoutItem {
public:
								BLayout();
								BLayout(BMessage* archive);
	virtual						~BLayout();

			BView*				Owner() const;
			BView*				TargetView() const;
	virtual	BView*				View(); // from BLayoutItem

	// methods dealing with items
	virtual	BLayoutItem*		AddView(BView* child);
	virtual	BLayoutItem*		AddView(int32 index, BView* child);

	virtual	bool				AddItem(BLayoutItem* item);
	virtual	bool				AddItem(int32 index, BLayoutItem* item);

	virtual	bool				RemoveView(BView* child);
	virtual	bool				RemoveItem(BLayoutItem* item);
	virtual	BLayoutItem*		RemoveItem(int32 index);

			BLayoutItem*		ItemAt(int32 index) const;
			int32				CountItems() const;
			int32				IndexOfItem(const BLayoutItem* item) const;
			int32				IndexOfView(BView* child) const;

			bool				AncestorsVisible() const;

	// Layouting related methods

	virtual	void				InvalidateLayout(bool children = false);
	virtual	void				Relayout(bool immediate = false);
			void				RequireLayout();
			bool				IsValid();
			void				EnableLayoutInvalidation();
			void				DisableLayoutInvalidation();

			void				LayoutItems(bool force = false);
			BRect				LayoutArea();
			BLayoutContext*		LayoutContext() const;

	// Archiving methods

	virtual status_t			Archive(BMessage* into, bool deep = true) const;
	virtual	status_t			AllUnarchived(const BMessage* from);

	virtual status_t			ItemArchived(BMessage* into, BLayoutItem* item,
									int32 index) const;
	virtual	status_t			ItemUnarchived(const BMessage* from,
									BLayoutItem* item, int32 index);

protected:
	// BLayout hook methods
	virtual	bool				ItemAdded(BLayoutItem* item, int32 atIndex);
	virtual	void				ItemRemoved(BLayoutItem* item, int32 fromIndex);
	virtual	void				DerivedLayoutItems() = 0;
	virtual	void				OwnerChanged(BView* was);

	// BLayoutItem hook methods
	virtual	void				AttachedToLayout();
	virtual void				DetachedFromLayout(BLayout* layout);
	virtual	void				AncestorVisibilityChanged(bool shown);

	// To be called by sub-classes in SetVisible().
			void				VisibilityChanged(bool show);
	// To be called when layout data is known to be good
			void				ResetLayoutInvalidation();

private:
			friend class BView;

			bool				RemoveViewRecursive(BView* view);
			bool				InvalidateLayoutsForView(BView* view);
			bool				InvalidationLegal();
			void				SetOwner(BView* owner);
			void				SetTarget(BView* target);

			void				_LayoutWithinContext(bool force,
									BLayoutContext* context);

			uint32				fState;
			bool				fAncestorsVisible;
			int32				fInvalidationDisabled;
			BLayoutContext*		fContext;
			BView*				fOwner;
			BView*				fTarget;
			BList				fItems;
			BList				fNestedLayouts;
};


#endif	//	_LAYOUT_H
*/
/*
#ifndef	_GROUP_LAYOUT_H
#define	_GROUP_LAYOUT_H

#include <TwoDimensionalLayout.h>

class BGroupLayout : public BTwoDimensionalLayout {
public:
								BGroupLayout(enum orientation orientation,
									float spacing = B_USE_DEFAULT_SPACING);
								BGroupLayout(BMessage* from);
	virtual						~BGroupLayout();

			float				Spacing() const;
			void				SetSpacing(float spacing);

			orientation			Orientation() const;
			void				SetOrientation(enum orientation orientation);
	
			float				ItemWeight(int32 index) const;
			void				SetItemWeight(int32 index, float weight);

	virtual	BLayoutItem*		AddView(BView* child);
	virtual	BLayoutItem*		AddView(int32 index, BView* child);
	virtual	BLayoutItem*		AddView(BView* child, float weight);
	virtual	BLayoutItem*		AddView(int32 index, BView* child,
									float weight);

	virtual	bool				AddItem(BLayoutItem* item);
	virtual	bool				AddItem(int32 index, BLayoutItem* item);
	virtual	bool				AddItem(BLayoutItem* item, float weight);
	virtual	bool				AddItem(int32 index, BLayoutItem* item,
									float weight);

	virtual status_t			Archive(BMessage* into, bool deep = true) const;
	virtual	status_t			AllUnarchived(const BMessage* from);
	static	BArchivable*		Instantiate(BMessage* from);

	virtual status_t			ItemArchived(BMessage* into, BLayoutItem* item,
									int32 index) const;
	virtual	status_t			ItemUnarchived(const BMessage* from,
									BLayoutItem* item, int32 index);

protected:	
	virtual	bool				ItemAdded(BLayoutItem* item, int32 atIndex);
	virtual	void				ItemRemoved(BLayoutItem* item, int32 fromIndex);

	virtual	void				PrepareItems(enum orientation orientation);
	
	virtual	int32				InternalCountColumns();
	virtual	int32				InternalCountRows();
	virtual	void				GetColumnRowConstraints(
									enum orientation orientation,
									int32 index,
									ColumnRowConstraints* constraints);
	virtual	void				GetItemDimensions(BLayoutItem* item,
									Dimensions* dimensions);

private:
			struct ItemLayoutData;

			ItemLayoutData*		_LayoutDataForItem(BLayoutItem* item) const;

			orientation			fOrientation;
			BList				fVisibleItems;
};

#endif	// _GROUP_LAYOUT_H
*/
/*
#ifndef	_GRID_LAYOUT_H
#define	_GRID_LAYOUT_H

#include <TwoDimensionalLayout.h>


class BGridLayout : public BTwoDimensionalLayout {
public:
								BGridLayout(float horizontal
										= B_USE_DEFAULT_SPACING,
									float vertical = B_USE_DEFAULT_SPACING);
								BGridLayout(BMessage* from);
	virtual						~BGridLayout();

			int32				CountColumns() const;
			int32				CountRows() const;

			float				HorizontalSpacing() const;
			float				VerticalSpacing() const;

			void				SetHorizontalSpacing(float spacing);
			void				SetVerticalSpacing(float spacing);
			void				SetSpacing(float horizontal, float vertical);

			float				ColumnWeight(int32 column) const;
			void				SetColumnWeight(int32 column, float weight);

			float				MinColumnWidth(int32 column) const;
			void				SetMinColumnWidth(int32 column, float width);

			float				MaxColumnWidth(int32 column) const;
			void				SetMaxColumnWidth(int32 column, float width);

			float				RowWeight(int32 row) const;
			void				SetRowWeight(int32 row, float weight);

			float				MinRowHeight(int row) const;
			void				SetMinRowHeight(int32 row, float height);

			float				MaxRowHeight(int32 row) const;
			void				SetMaxRowHeight(int32 row, float height);

	virtual	BLayoutItem*		AddView(BView* child);
	virtual	BLayoutItem*		AddView(int32 index, BView* child);
	virtual	BLayoutItem*		AddView(BView* child, int32 column, int32 row,
									int32 columnCount = 1, int32 rowCount = 1);

	virtual	bool				AddItem(BLayoutItem* item);
	virtual	bool				AddItem(int32 index, BLayoutItem* item);
	virtual	bool				AddItem(BLayoutItem* item, int32 column,
									int32 row, int32 columnCount = 1,
									int32 rowCount = 1);

	virtual	status_t			Archive(BMessage* into, bool deep = true) const;
	static	BArchivable*		Instantiate(BMessage* from);

	virtual status_t			ItemArchived(BMessage* into,
									BLayoutItem* item, int32 index) const;
	virtual status_t			ItemUnarchived(const BMessage* from,
									BLayoutItem* item, int32 index);

protected:	
	virtual	bool				ItemAdded(BLayoutItem* item, int32 atIndex);
	virtual	void				ItemRemoved(BLayoutItem* item, int32 fromIndex);

	virtual	bool				HasMultiColumnItems();
	virtual	bool				HasMultiRowItems();
	
	virtual	int32				InternalCountColumns();
	virtual	int32				InternalCountRows();
	virtual	void				GetColumnRowConstraints(
									enum orientation orientation,
									int32 index,
									ColumnRowConstraints* constraints);
	virtual	void				GetItemDimensions(BLayoutItem* item,
									Dimensions* dimensions);

private:	
			class DummyLayoutItem;
			class RowInfoArray;
			struct ItemLayoutData;
			
			bool				_IsGridCellEmpty(int32 column, int32 row);
			bool				_AreGridCellsEmpty(int32 column, int32 row,
									int32 columnCount, int32 rowCount);

			bool				_InsertItemIntoGrid(BLayoutItem* item);
			bool				_ResizeGrid(int32 columnCount, int32 rowCount);

			ItemLayoutData*		_LayoutDataForItem(BLayoutItem* item) const;

private:
			BLayoutItem***		fGrid;
			int32				fColumnCount;
			int32				fRowCount;

			RowInfoArray*		fRowInfos;
			RowInfoArray*		fColumnInfos;

			int32				fMultiColumnItems;
			int32				fMultiRowItems;
};

#endif	// _GRID_LAYOUT_H
*/
