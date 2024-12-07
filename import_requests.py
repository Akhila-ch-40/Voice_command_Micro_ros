import requests
import pyttsx3  # For feedback through speech
import speech_recognition as sr

# Replace with the IP address of your ESP32
ESP32_IP = "http://192.168.50.23"

# Initialize text-to-speech engine
engine = pyttsx3.init()

def send_signal(signal):
    try:
        # Map signal to GPIO command
        url = f"{ESP32_IP}/GPIO={signal}"
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Signal {signal} sent successfully.")
            engine.say(f"Signal {signal} sent successfully.")
        else:
            print(f"Failed to send signal {signal}: {response.status_code}")
            engine.say(f"Failed to send signal {signal}.")
        engine.runAndWait()
    except Exception as e:
        print(f"Error sending signal {signal}: {e}")
        engine.say(f"Error sending signal {signal}.")
        engine.runAndWait()

def recognize_speech():
    recognizer = sr.Recognizer()
    microphone = sr.Microphone()

    print("Listening for commands... (say 'right', 'left', 'front', or 'back')")

    while True:
        try:
            with microphone as source:
                recognizer.adjust_for_ambient_noise(source)  # Adjust to background noise
                audio = recognizer.listen(source)           # Listen for audio input

            # Recognize speech using Google Web Speech API (online)
            command = recognizer.recognize_google(audio).lower()

            if "right" in command:
                print("Command recognized: right")
                send_signal("1")
            elif "left" in command:
                print("Command recognized: left")
                send_signal("2")
            elif "front" in command:
                print("Command recognized: front")
                send_signal("3")
            elif "back" in command:
                print("Command recognized: back")
                send_signal("4")
            elif "quit" in command or "exit" in command:
                print("Exiting...")
                engine.say("Exiting the program.")
                engine.runAndWait()
                break
            else:
                print(f"Unrecognized command: {command}")
                engine.say(f"Unrecognized command: {command}")
                engine.runAndWait()
        except sr.UnknownValueError:
            print("Could not understand the audio. Please try again.")
            engine.say("Could not understand the audio. Please try again.")
            engine.runAndWait()
        except sr.RequestError as e:
            print(f"Could not request results from the speech recognition service; {e}")
            engine.say("There was an error with the speech recognition service.")
            engine.runAndWait()

def main():
    print("Speak 'right' to send signal 1")
    print("Speak 'left' to send signal 2")
    print("Speak 'front' to send signal 3")
    print("Speak 'back' to send signal 4")
    print("Speak 'quit' or 'exit' to end the program.")
    recognize_speech()

if __name__ == "__main__":
    main()
