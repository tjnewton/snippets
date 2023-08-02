import tkinter as tk
from tkinter import simpledialog
import requests
import matplotlib.pyplot as plt
from datetime import datetime, timedelta
import matplotlib.dates as mdates
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

API_KEY = "5bb807aa6801fb968f03ff422480c568"

def get_weather(zip_code):
    response = requests.get(f"http://api.openweathermap.org/data/2.5/forecast?zip={zip_code},us&appid={API_KEY}&units=imperial")
    return response.json()

def predict_mosquito_activity(weather_data):
    mosquito_activity = []
    weather_vars = {"temp": [], "humidity": [], "wind_speed": [], "rain": []}
    for forecast in weather_data["list"]:
        temp = forecast["main"]["temp"]  # Now in Fahrenheit
        humidity = forecast["main"]["humidity"]
        wind_speed = forecast["wind"]["speed"]  # Now in mph
        rain = forecast["rain"]["3h"] if "rain" in forecast and "3h" in forecast["rain"] else 0  # Now in inches
        rain *= 10  # Exaggerate rain value
        dt_txt = forecast["dt_txt"]
        hour = int(dt_txt.split(" ")[1].split(":")[0])

        # Rough estimation of mosquito activity based on the temperature, humidity, wind speed, and precipitation
        activity = temp + humidity - (wind_speed * 10) + (rain * 10)

        # Mosquitos are less active during daylight hours
        if 7 <= hour <= 19:
            activity -= 20

        mosquito_activity.append((dt_txt, activity))
        weather_vars["temp"].append((dt_txt, temp))
        weather_vars["humidity"].append((dt_txt, humidity))
        weather_vars["wind_speed"].append((dt_txt, wind_speed))
        weather_vars["rain"].append((dt_txt, rain))

    return mosquito_activity, weather_vars

def plot_activity(mosquito_activity, weather_vars):
    times = [datetime.strptime(m[0], '%Y-%m-%d %H:%M:%S') for m in mosquito_activity]
    activities = [m[1] for m in mosquito_activity]
    fig = plt.figure(figsize=(16, 6))

    # Plot weather variables
    plt.plot_date(times, [t[1] for t in weather_vars["temp"]], '-', label='Temp (F)', alpha=0.3)
    plt.plot_date(times, [h[1] for h in weather_vars["humidity"]], '-', label='Humidity (%)', alpha=0.3)
    plt.plot_date(times, [w[1] for w in weather_vars["wind_speed"]], '-', label='Wind Speed (mph)', alpha=0.3)
    plt.plot_date(times, [r[1] for r in weather_vars["rain"]], '-', label='Rain (inches x 10)', alpha=0.3)

    # Plot mosquito activity
    plt.plot_date(times, activities, '-', label='Mosquito Activity', linewidth=3.0)

    # Add lines at 00:00 each day
    min_date = times[0].date()
    max_date = times[-1].date()
    delta = timedelta(days=1)
    while min_date <= max_date:
        plt.axvline(x=min_date, color='r', linestyle='-', linewidth=2)
        plt.text(min_date, plt.ylim()[0], min_date.strftime('%A'), rotation=90, color='r', verticalalignment='bottom')
        min_date += delta

    plt.gcf().autofmt_xdate()  # Beautify the x-labels
    plt.autoscale(tight=True)
    plt.grid(True)
    plt.title('Predicted Mosquito Activity and Weather Variables')
    plt.ylabel('Estimated Activity Level')
    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))  # Show only time, not date
    plt.gca().xaxis.set_major_locator(mdates.HourLocator(interval=6))
    plt.legend()

    return fig

def generate_plot():
    zip_code = simpledialog.askstring("Location", "Enter your zip code:")
    weather_data = get_weather(zip_code)
    mosquito_activity, weather_vars = predict_mosquito_activity(weather_data)
    fig = plot_activity(mosquito_activity, weather_vars)
    
    canvas = FigureCanvasTkAgg(fig, master=root)
    canvas.draw()
    canvas.get_tk_widget().pack()

root = tk.Tk()
button = tk.Button(root, text='Predict Mosquito Activity', command=generate_plot)
button.pack()

root.mainloop()

