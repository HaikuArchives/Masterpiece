#!/boot/home/config/bin/python

import BApplication
import BitView

from BView import BView
from BWindow import BWindow
from BButton import BButton
from BMessage import BMessage
from BFont import be_bold_font
from AppKit import B_QUIT_REQUESTED

import InterfaceKit as IK

class MasterWindow(BWindow):
	def __init__(self, node):
		BWindow.__init__(self, (100.0, 80.0, 640.0, 480.0), 'MasterPiece', 
			IK.B_TITLED_WINDOW, IK.B_CURRENT_WORKSPACE)
		self.WindowBounds = self.Bounds()
		self.view = BView((0.0, 0.0, self.WindowBounds[2], 30.0), 'MasterToolBarView', IK.B_FOLLOW_LEFT_RIGHT | IK.B_FOLLOW_TOP, IK.B_WILL_DRAW)
		self.newMPButton = BButton((0.0, 0.0, 120.0, 22.0), "newButton", "New MasterPiece", BMessage(1), IK.B_FOLLOW_LEFT, IK.B_WILL_DRAW)
		self.iconBox = BitView.BitView((0.0, 0.0, 22.0, 22.0), node)
		# reference showicon for example of picture button that i can replicate here...
		self.backColor = (215, 215, 215, 255)
		self.view.SetViewColor(self.backColor)
		#self.view.AddChild(self.newMPButton)
		self.view.AddChild(self.iconBox)
		
		self.AddChild(self.view)
	
	def QuitRequested(self):
		BApplication.be_app.PostMessage(B_QUIT_REQUESTED)
		return 1
# the current method actually get's the tracker icon, not the actual file information...
# may have to try the BFile to manipulate it more than the current method. or just use menu's and forget all the
# icon stuff...
