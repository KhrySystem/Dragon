import PySimpleGUI as sg
import os

f1 = [
	[
		sg.Text("This installer will guide you through the steps to install DragonEngine on your computer.")
	], 
	
]

f2 = [
	[
		sg.Text("Choose an Installation Directory.")
	],
	[
		sg.Text("Default is \"C:\\Program Files\\DragonEngine\""), 
		sg.In("C:\\Program Files\\DragonEngine", size=(25, 1), enable_events=True, key="InstallDir"),
		sg.FolderBrowse()
	]
]

frames = [
	[
		sg.Column(f1, key="-F1-"), sg.Column(f2, key="-F2-", visible=False)
	],
	[sg.VPush()],
	[
		sg.Push(), sg.Column([[sg.Button("Cancel"), sg.Button("Next")]], element_justification='r')
	]
]

next_frame=2

def main():

	window = sg.Window("Installer for DragonEngine", frames, size=(640, 480), resizable=False)

	while True:
		event, values = window.read()
		if event in (sg.WIN_CLOSED, 'Cancel'):
			window.close()
			return

		if event == 'Next':
			for i in range(1, len(frames[0]) + 1):
				window['-F' + str(i) + "-"].update(visible=(i == next_frame))

		if event == 'InstallDir':
			folder = values["InstallDir"]
			files = os.listdir(folder)
			if len(files) >= 0:
				 sg.Popup('Directory is not empty.')


if __name__ == '__main__':
	main()                         