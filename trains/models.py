from django.db import models
from django.utils import timezone

class Train(models.Model):
	title = models.CharField(max_length=200)
	description = models.TextField()
	address = models.PositiveSmallIntegerField()
	speed = models.PositiveSmallIntegerField()
	direction = models.BooleanField()
	soundOptions = models.PositiveSmallIntegerField()
	image = models.ImageField()
	created_date = models.DateTimeField(
            default=timezone.now)
			
	def __str__(self):
		return self.title