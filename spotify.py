import serial
import time
import spotipy
from spotipy.oauth2 import SpotifyOAuth

#this script must run continuously in background for controller to work


ser = serial.Serial('arduino_port', 9600, timeout=1) #arduino port example: /dev/cu.usbmodem1201

time.sleep(2)  



ACCESS_TOKEN = 'spotify_api_access_token'
#https://developer.spotify.com/documentation/web-api/concepts/access-token 


sp = spotipy.Spotify(auth=ACCESS_TOKEN)


line = None


try:
    while True:
        if(sp.current_playback == None):
            ser.write(b' ')
            time.sleep(1)
            continue
   
       # if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        if(line == 'next'):
            sp.next_track()
        if(line == 'prev'):
            sp.previous_track()
        if(line == 'pause'):
            if(sp.current_playback().get('is_playing')):
                sp.pause_playback()
            else:
                sp.start_playback()
            
        ser.flushInput()
            
        if(sp.current_user_playing_track() == None):
            ser.write(b' ')
            time.sleep(1)
            continue
        
        mes = (sp.current_user_playing_track().get('item').get('name'))[0:16] + "|"+(sp.current_user_playing_track().get('item').get('artists')[0].get('name'))[0:16]

        ser.write(mes.encode())
        time.sleep(1) 
except KeyboardInterrupt:
    ser.close()
except Exception as e:
    print(f"An error occurred: {e}")
    ser.close()
