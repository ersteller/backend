
docker run -d --rm --name qpid -p 5672:5672 irinabov/docker-qpid-cpp-broker
docker exec -it qpid qpid-config add queue examples
