# Django_Learning_Notes
Created in 2017/2/16 by YunShuiXin

## Writing first Django app
---
tutorial from [Django Documentation](https://docs.djangoproject.com/en/1.10/intro/tutorial01/)
+ test whether django is installed `python -m django --version`
+ startproject `django-admin startproject mysite`
> project name should not contain "django" or "test"

`cd mysite`

`python3 manage.py runserver`

 + to change the server port run `python3 manage.py runserver 8080`
 + to change the server ip run `python3 manage.py runserver 0.0.0.0:8000`
 + in the directory of manage.py run `python3 manage.py startapp polls`
 +
