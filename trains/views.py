from django.shortcuts import render
from django.utils import timezone
from django.http import HttpResponse
from .models import Train, Light
from django.views.decorators.csrf import csrf_exempt
import redis

def train_list(request):
	trains = Train.objects.all()
	return render(request, 'trains/train_list.html', {'trains': trains})


def main_page(request):
	trains = Train.objects.all()
	lights = Light.objects.all()

	return render(request, 'trains/index.html', {'trains': trains, 'lights' : lights})
	
@csrf_exempt
def command_ajax(request):
	if request.method == 'POST':
		datatype = request.POST.get('datatype')
		
		r = redis.StrictRedis(host='localhost', port=6379)
		p = r.pubsub()
		
		
		if datatype == "command":
			data = request.POST.get('command')
			print("GOT This: " + data)
			r.publish('buttonPress', data)
		elif datatype == "hex":
			data = request.POST.get('data')
			r.publish('buttonPress', data)
			print("Hex value;" + data)
		return HttpResponse("ok")

@csrf_exempt
def command_ajax_trains(request):
	if request.method == 'POST':
		datatype = request.POST.get('datatype')
		
		r = redis.StrictRedis(host='localhost', port=6379)
		p = r.pubsub()
		
		
		if datatype == "command":
			data = request.POST.get('command')
			print("GOT This: " + data)
			r.publish('trainData', data)
		return HttpResponse("ok")