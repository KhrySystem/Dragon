import pysimplegui as sg

intro = [
    [
        sg.Column([
            [sg.Image('res/logo_64.png')]
    ], expand_y=True, background_color="gray50"), 
    sg.Column([[
        sg.Text("This will guide you through installing DragonEngine on your computer.", background_color="white", text_color="black")
    ],[sg.Text("", expand_y=True, background_color="white")]
    ], background_color="white")], 
    [sg.Text("", expand_x=True, background_color="white"), sg.Button("Cancel", button_color="grey50"), sg.Button("Next", button_color="grey50")]
]

window = sg.Window("DragonEngine Installer", intro, size=(400, 300), background_color='white')

while True:
    event, values = window.read()
    if event == sg.WINDOW_CLOSED or event == "Cancel":
        break

window.close()