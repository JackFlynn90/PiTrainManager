from django.shortcuts import render
from django.utils import timezone
from .models import Train, Light

def train_list(request):
	trains = Train.objects.all()
	return render(request, 'trains/train_list.html', {'trains': trains})
	
def main_page(request):
	trains = Train.objects.all()
	lights = Light.objects.all()
	return render(request, 'trains/index.html', {'trains': trains, 'lights' : lights})