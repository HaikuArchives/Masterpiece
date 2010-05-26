from BBitmap import BBitmap
from BBox import BBox
from BNodeInfo import BNodeInfo

import InterfaceKit as IK

class BitView(BBox):
	def __init__(self, frame, node):
		BBox.__init__(self, frame, 'bitbox', IK.B_FOLLOW_ALL,
			IK.B_WILL_DRAW, IK.B_PLAIN_BORDER)
		self.bits = BBitmap((0, 0, 31, 31), IK.B_CMAP8)
		ni = BNodeInfo(node)
		ni.GetTrackerIcon(self.bits)
	def Draw(self, rect):
		BBox.Draw(self, rect)
		inset = (0, 0, 22, 22)
		self.DrawBitmap(self.bits, inset)
