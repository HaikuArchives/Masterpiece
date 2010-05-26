#!/boot/home/config/bin/python

import BApplication
import MasterWindow

from BNode import BNode
from AppKit import B_QUIT_REQUESTED

class MasterPieceApplication(BApplication.BApplication):
	def __init__(self):
		BApplication.BApplication.__init__(self,
			"application/x-vnd.pjr-MasterPiece")
	def ReadyToRun(self):
		node = BNode("images/document-new.png")
		window = MasterWindow.MasterWindow(node)
		window.Show()

MasterPieceApp = MasterPieceApplication()
MasterPieceApp.Run()
