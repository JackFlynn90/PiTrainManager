from django.db import models
from django.utils import timezone

class Train(models.Model):
	title = models.CharField(max_length=200)
	description = models.TextField()
	address = models.PositiveSmallIntegerField()
	speed = models.PositiveSmallIntegerField()
	direction = models.BooleanField()
	lightsOn = models.BooleanField(default=True)
	hornIs = models.PositiveSmallIntegerField(default=1)
	silentRunning = models.BooleanField(default=False)
	soundOptions = models.PositiveSmallIntegerField()
	image = models.ImageField()
	created_date = models.DateTimeField(
            default=timezone.now)
	def __str__(self):
		return self.title
		
class Light(models.Model):
	title = models.CharField(max_length=200)
	description = models.TextField()
	address = models.PositiveSmallIntegerField()
	type = models.CharField(max_length=200)
	brightness = models.PositiveSmallIntegerField()
	colour = models.PositiveSmallIntegerField()
	lightsState = models.BooleanField(default=True)
	hexValue = models.CharField(max_length=200)
	created_date = models.DateTimeField(
            default=timezone.now)
	def __str__(self):
		return self.title
		
class Functions(models.Model):
	title = models.CharField(max_length=200)
	description = models.TextField()
	train = models.ForeignKey(
        Train,
        on_delete=models.CASCADE,)
	address = models.PositiveSmallIntegerField()
	constant_send = models.BooleanField()
	current_state = models.BooleanField()
	is_active = models.BooleanField()

	created_date = models.DateTimeField(
            default=timezone.now)
	def __str__(self):
		return self.title
		