#!/boot/home/config/bin/python

import BApplication

from BView import BView
from BWindow import BWindow
from BButton import BButton
from BMessage import BMessage
from BMenuBar import BMenuBar
from BMenu import BMenu
from BMenuItem import BMenuItem
from BSeparatorItem import BSeparatorItem

from BFont import be_bold_font
from AppKit import B_QUIT_REQUESTED

import InterfaceKit as IK

class MasterWindow(BWindow):
	def __init__(self):
		BWindow.__init__(self, (100.0, 80.0, 640.0, 480.0), 'MasterPiece', 
			IK.B_TITLED_WINDOW, IK.B_CURRENT_WORKSPACE)
		self.WindowBounds = self.Bounds()
		self.view = BView((0.0, 0.0, self.WindowBounds[2], 18.0), 'MasterToolBarView', IK.B_FOLLOW_LEFT_RIGHT | IK.B_FOLLOW_TOP, IK.B_WILL_DRAW)
		self.mainMenuBar = BMenuBar(self.WindowBounds, 'MenuBar')
		self.fileMenu = BMenu('File')
		self.newMenuItem = BMenuItem('New MasterPiece', BMessage(1))
		self.quitMenuItem = BMenuItem('Quit', BMessage(B_QUIT_REQUESTED))
		self.fileMenu.AddItem(self.newMenuItem)
		self.fileMenu.AddItem(BSeparatorItem())
		self.fileMenu.AddItem(self.quitMenuItem)
		self.mainMenuBar.AddItem(self.fileMenu)
		x, barheight = self.mainMenuBar.GetPreferredSize()
		self.backColor = (215, 215, 215, 255)
		self.view.SetViewColor(self.backColor)
		self.view.AddChild(self.mainMenuBar)
		self.AddChild(self.view)
		self.newWindow = NewMasterWindow()
	
	def MessageReceived(self, msg):
		if msg.what == 1:
			print 'do something here'
			self.newWindow.Show()
		BWindow.MessageReceived(self, msg)
	
	def QuitRequested(self):
		BApplication.be_app.PostMessage(B_QUIT_REQUESTED)
		return 1

class NewMasterWindow(BWindow):
	def __init__(self):
		BWindow.__init__(self, (100.0, 80.0, 350.0, 200.0), 'Start New MasterPiece', IK.B_MODAL_WINDOW, IK.B_CURRENT_WORKSPACE)
		self.WindowBounds = self.Bounds()
		self.view = BView((0.0, 0.0, self.WindowBounds[2], self.WindowBounds[3]), 'NewDocumentView', IK.B_FOLLOW_ALL, IK.B_WILL_DRAW)
		self.backColor = (215, 215, 215, 255)
		self.view.SetViewColor(self.backColor)
		self.AddChild(self.view)
		
	def QuitRequested(self):
		self.Close()
