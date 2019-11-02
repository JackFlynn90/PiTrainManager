from django.contrib import admin

from .models import Train, Light, Functions, Servo, LightGroup

admin.site.register(Train)
admin.site.register(Light)
admin.site.register(Servo)
admin.site.register(Functions)
admin.site.register(LightGroup)
