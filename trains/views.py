from django.shortcuts import render
from django.utils import timezone
from django.http import HttpResponse
from .models import Train, Light, Servo
from django.views.decorators.csrf import csrf_exempt
import redis

def train_list(request):
	trains = Train.objects.all()
	return render(request, 'trains/train_list.html', {'trains': trains})


def main_page(request):
	trains = Train.objects.all()
	lights = Light.objects.all()
	servos = Servo.objects.all()
	
	return render(request, 'trains/index.html', {'trains': trains, 'lights' : lights, 'servos':servos})
	
@csrf_exempt
def command_ajax(request):
	if request.method == 'POST':
		datatype = request.POST.get('datatype')
		
		r = redis.StrictRedis(host='localhost', port=6379)
		p = r.pubsub()
		
		
		if datatype == "command":
			data = request.POST.get('command')
			print("Effects got This: " + data)
			r.publish('EffectsCommand', data)
		elif datatype == "hex":
			data = request.POST.get('data')
			r.publish('EffectsCommand', data)
			print("Effects got This Hex value;" + data)
		elif datatype == "servo":
			data = request.POST.get('servo')
			r.publish('EffectsCommand', data)
			print("Servo Effects got This value;" + data)
		return HttpResponse("ok")

@csrf_exempt
def command_ajax_trains(request):
	if request.method == 'POST':
		datatype = request.POST.get('datatype')
		
		r = redis.StrictRedis(host='localhost', port=6379)
		p = r.pubsub()
		
		
		if datatype == "command":
			data = request.POST.get('command')
			print("Trains GOT This: " + data)
			r.publish('trainCommand', data)
		return HttpResponse("ok")