FROM python:3.9.17

COPY requirements.txt /tmp/requirements.txt

RUN apt update && apt install -y python3-pip

RUN python3 -m pip install -r /tmp/requirements.txt
