#!/boot/home/config/bin/python

import BApplication
import MasterWindow

from AppKit import B_QUIT_REQUESTED

class MasterPieceApplication(BApplication.BApplication):
	def __init__(self):
		BApplication.BApplication.__init__(self,
			"application/x-vnd.pjr-MasterPiece")
	def ReadyToRun(self):
		window = MasterWindow.MasterWindow()
		window.Show()

MasterPieceApp = MasterPieceApplication()
MasterPieceApp.Run()
