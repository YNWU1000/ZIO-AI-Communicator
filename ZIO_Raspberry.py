import os
import io
import sys
import asyncio
import argparse
import pyaudio
import wave
from RPi import GPIO

from google.cloud import speech
from google.cloud import texttospeech

import openai
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 19200)


#keys, do not copy or share
os.environ['GOOGLE_APPLICATION_CREDENTIALS']='GoogleCloud.json'
openai.api_key = "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"

#ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1 )

GPIO.setmode(GPIO.BOARD)
#set up the pins we have been using
#clk = 40
#dt = 38
swr = 12
swm = 15
 
#set up the GPIO events on those pins
#GPIO.setup(clk, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
#GPIO.setup(dt, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

GPIO.setup(swr, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(swm, GPIO.IN, pull_up_down=GPIO.PUD_UP)

running = False


#/////////////////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////////////////

def speech_to_text(speech_file):
    #google cloud speech-to-text API configs
    client = speech.SpeechClient()
    #read the audio file""speech_file" which is gonna be recorded through mic
    with io.open(speech_file, "rb") as audio_file:
            content = audio_file.read()
    audio = speech.RecognitionAudio(content=content)

    #config the speech-to-text engine
    config = speech.RecognitionConfig(
        encoding=speech.RecognitionConfig.AudioEncoding.LINEAR16,
        enable_automatic_punctuation=True,
        language_code="en-US",
        #alternativeLanguageCodes["zh(cmn-Hans-CN)","zh-TW(cmn-Hant-tw)","ja-JP", "ko-KR", "es-MX", "es-US", "pt-BR"],
    )
    
    # Detects speech in the audio file
    response = client.recognize(config=config, audio=audio)

    # get transcribation from google cloud
    stt = ""
    for result in response.results:
        stt += result.alternatives[0].transcript

    return stt

#/////////////////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////////////////

def GPT(prompt):
    global gpt_response
    
    #messages = []
    #system_msg = "You talk like Kurt Cobain, always response in no more than 50 words. Do not mention you are an AI model. "
    #messages.append({"role": "system", "content": system_msg})

    messages = [
        {"role": "system", "content": "You are passionate, futuristic and like to reference facts. Always response in one short paragraph close to but never longer than 35 words, no more than 256 characters. If possible, always use shorter words as long as it does not lose the quality of information. "},
        {"role": "user", "content": prompt}
        ]
    response = openai.ChatCompletion.create(
        model="gpt-4",
        messages=messages)
    reply = response["choices"][0]["message"]["content"]
    messages.append({"role": "assistant", "content": reply})
    
    gpt_response = reply
     
    return


#/////////////////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////////////////
#pyaudio configs, microphone
def record_wav():
    form_1 = pyaudio.paInt16
    chans = 1
    samp_rate = 16000
    chunk = 4096
    record_secs = 9
    dev_index = 2
    wav_output_filename = 'input.wav'

    audio = pyaudio.PyAudio()

    # Create pyaudio stream.
    stream = audio.open(format = form_1,rate = samp_rate,channels = chans, \
                        input_device_index = dev_index,input = True, \
                        frames_per_buffer=chunk)
    print("Start speaking now:")
    frames = []

    # Loop through stream and append audio chunks to frame array.
    for ii in range(0,int((samp_rate/chunk)*record_secs)):
        data = stream.read(chunk)
        frames.append(data)

    print("finished recording")

    # Stop the stream, close it, and terminate the pyaudio instantiation.
    stream.stop_stream()
    stream.close()
    audio.terminate()

    # Save the audio frames as .wav file.
    wavefile = wave.open(wav_output_filename,'wb')
    wavefile.setnchannels(chans)
    wavefile.setsampwidth(audio.get_sample_size(form_1))
    wavefile.setframerate(samp_rate)
    wavefile.writeframes(b''.join(frames))
    wavefile.close()

    return

#/////////////////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////////////////

def text_to_speech(tts):
    # Instantiates a client
    client = texttospeech.TextToSpeechClient()

    # Set the text input to be synthesized
    synthesis_input = texttospeech.SynthesisInput(text=tts)

    # Build the voice request, select the language code ("en-US") and the ssml
    #name: cmn-CN-Standard-A, cmn-CN-Standard-D pitch +-8, ja-JP-Standard-A Pitch5, en-US-Neural2-I Pitch -6; ru-RU-Standard-E, he-IL-standard-B pitch -5, 
    voice = texttospeech.VoiceSelectionParams(
        language_code="en-US", ssml_gender=texttospeech.SsmlVoiceGender.MALE, name="en-US-Neural2-I"
    )

    # Select the type of audio file you want returned
    audio_config = texttospeech.AudioConfig(
        pitch = -6.7, audio_encoding=texttospeech.AudioEncoding.LINEAR16
    )

    # Perform the text-to-speech request on the text input with the selected
    # voice parameters and audio file type
    response = client.synthesize_speech(
        input=synthesis_input, voice=voice, audio_config=audio_config
    )

    # The response's audio_content is binary.
    with open("result.wav", "wb") as out:
        # Write the response to the output file.
        out.write(response.audio_content)

    return

#/////////////////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////////////////

def main():
    
    #ser.write("/".encode('utf-8'))
    
    record_wav()

    question = speech_to_text("input.wav")
    
    print("Asking: {0}".format(question))
    
    GPT(question)
    
    Humanquestion = question
    
    gpt_response2 = gpt_response
    LCDSerial = gpt_response2
    
    ser.write((Humanquestion + "|").encode('utf-8'))
    ser.write((LCDSerial + "\n").encode('utf-8'))
    
    print("Response: {0}".format(gpt_response2))
    
    text_to_speech(gpt_response)
    
    os.system("aplay result.wav")
    
    time.sleep(15.0)
    ser.close()
    time.sleep(0.2)
    ser.open()
    

    
#/////////////////////////////////////////////////////////////////////////////
try:
    while True:
        swm_state = GPIO.input(swm)
        swr_state = GPIO.input(swr)
    
        if swm_state ==False and not running:
            print("pushed and starting!")
            running =True
            #ser.write("/".encode('utf-8'))
            main()
            running = False
            time.sleep(0.5)
        elif swr_state ==False:
            print("EXITED")
            time.sleep(0.5)
            break
        else:
            time.sleep(0.1)
except KeyboardInterrupt:
    print("ending chat.")
#     keyboard_input = input("Press S to start a chat, Q to quit, R to continue. \n")
#     if keyboard_input == "s":
#         main()
#     elif keyboard_input == 'q':
#         break
#     elif keyboard_input == 'r':
#         continue
#     else:
#         print("LOL error")