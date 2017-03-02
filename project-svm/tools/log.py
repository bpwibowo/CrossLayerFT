import logging
import sys

FORMAT_RED_BOLD = "\033[1;31m"
FORMAT_GREEN_BOLD = "\033[1;32m"
FORMAT_YELLOW_BOLD = "\033[1;33m"
FORMAT_BLUE_BOLD = "\033[1;34m"
FORMAT_CLEAR = "\033[0m"

# Custom formatter
class MyFormatter(logging.Formatter):

	def __init__(self, prefix, fmt="%(levelno)s: %(msg)s"):
		logging.Formatter.__init__(self, fmt)
		self.prefix = prefix

	def format(self, record):

		# Save the original format configured by the user
		# when the logger formatter was instantiated
		format_orig = self._fmt

		# Replace the original format with one customized by logging level
		if record.levelno == logging.DEBUG:
			self._fmt = FORMAT_GREEN_BOLD + "%(asctime)s " + \
				"[" + self.prefix + "-DEBUG] %(module)s:%(lineno)d: %(msg)s" + \
				FORMAT_CLEAR
		elif record.levelno == logging.INFO:
			self._fmt = FORMAT_BLUE_BOLD + "%(asctime)s " + \
				"[" + self.prefix + "-INFO] %(msg)s" + \
				FORMAT_CLEAR
		elif record.levelno == logging.WARNING:
			self._fmt = FORMAT_YELLOW_BOLD + "%(asctime)s " + \
				"[" + self.prefix + "-WARNING] %(msg)s" + \
				FORMAT_CLEAR
		elif record.levelno == logging.ERROR:
			self._fmt = FORMAT_RED_BOLD + "%(asctime)s " + \
				"[" + self.prefix + "-ERROR] %(msg)s" + \
				FORMAT_CLEAR

		# Call the original formatter class to do the grunt work
		result = logging.Formatter.format(self, record)

		# Restore the original format configured by the user
		self._fmt = format_orig

		return result

def Init(prefix, level):
	fmt = MyFormatter(prefix)
	hdlr = logging.StreamHandler(sys.stdout)

	hdlr.setFormatter(fmt)
	logging.root.addHandler(hdlr)
	logging.root.setLevel(level)