#!python
# TODO:
#   - Only update during market hours
#   - Only alert for each stock once per a longer configurable timeframe
#   - Handle errors that get returned as JSON

import requests
import time
import ctypes
MessageBoxW = ctypes.windll.user32.MessageBoxW

# Basic config
API_KEY = "api_key_here"
API_URL = "https://www.worldtradingdata.com/api/v1/stock?api_token=" + API_KEY
UPDATE_FREQ = 2.5 * 60
watchlist = {
	"MSFT":1.00
}

symbolArgs = "&symbol="
for key in watchlist:
	symbolArgs = symbolArgs + key + ','
# Trim trailing ,
symbolArgs = symbolArgs[:-1]
#print(API_URL + symbolArgs)

while True:
	print()
	continueProcessing = True
	req = requests.get(API_URL + symbolArgs)
	if (req.status_code == 200):
		try:
			reqData = req.json()
		except:
			continueProcessing = False
			print("Requests error: Could not decode as JSON. Server said:")
			print(req.text)

		if (continueProcessing):
			for ticker in reqData["data"]:
				if (float(ticker["price"]) > watchlist[ticker["symbol"]]):
					compstr = " (higher)"
				else:
					compstr = " (!!)"
					boxstr = ticker["symbol"] + ":\r\n    " + \
						"Target price: " + str(watchlist[ticker["symbol"]]) + "\r\n    " + \
						"Current price: " + str(ticker["price"])
					MessageBoxW(None, boxstr, "Stock Price Alert", 0)
				print(ticker["symbol"] + ": " + ticker["price"] + compstr)

	else:
		print("Requests error: Status code " + str(req.status_code))
	time.sleep(UPDATE_FREQ)
#	break
