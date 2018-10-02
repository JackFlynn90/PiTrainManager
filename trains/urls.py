from django.urls import path
from django.conf import settings
from django.conf.urls.static import static
from . import views

urlpatterns = [
	path('', views.main_page, name='main_page'),
    path('trains/', views.train_list, name='train_list'),
	path('commands/', views.command_ajax, name='command_ajax'),
	path('command_trains/', views.command_ajax_trains, name='command_ajax_trains'),
	
]