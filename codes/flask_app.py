from flask import Flask, Response, render_template
from flask import request
import json
import time
from datetime import datetime
from flask_googlemaps import GoogleMaps, Map

app = Flask(__name__)

api_key = "PUT IN API KEY"
GoogleMaps(app, key=api_key) # set api_key

content = []
timestamps = "0"
count = 1
values = []
temp = []
altitude = 20
longitude = 0.0
latitude = 0.0

@app.route('/')
def esphello():
    return "IoT Project Demo"

@app.route('/graph')
def amplitude():
    return render_template('index.html')

@app.route('/chart-data')
def chart_data():
    global altitude,timestamps
    def generate_random_data():
        while True:
            json_data = json.dumps(
                {'time': datetime.now().strftime('%H:%M:%S'), 'value': altitude})
            yield f"data:{json_data}\n\n"
            time.sleep(10)

    return Response(generate_random_data(), mimetype='text/event-stream')

@app.route('/postjson', methods = ['POST'])
def postJsonHandler():
    global count,content,values,timestamps,temp,altitude,latitude,longitude
    content = request.get_json()
    temp = content["values"]
    timestamps = content["timestamps"][0]
    temp.append(timestamps)
    altitude = content["values"][2]
    latitude = content["values"][0]
    longitude = content["values"][1]
    values.append(temp)
    count += 1
    #print (content)
    return "Message Received."


@app.route('/getjson')
def getJsonHandler():
    global content,latitude,longitude
    return str(content)

#'''
@app.route('/map')
def maps():
    global latitude,longitude
    map = Map(
        identifier = "map", varname = "map",
        # set identifier, varname
        lat = latitude, lng = longitude,
        # set map base to user_location
        zoom = 15, # set zoomlevel
        markers = [
            {
                'lat': latitude,
                'lng': longitude,
                'infobox': "Drone Current Location"
            }
        ]
    )

    return render_template('map.html', map=map) # render template
#'''
