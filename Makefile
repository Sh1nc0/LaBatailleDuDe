ifdef debug
	DIR = linux/debug
	SUFFIX = _d
else
	DIR = linux/release
	SUFFIX =
endif

ifdef podman
	DOCKER = podman
else
	DOCKER = docker
endif

export debug
export podman

WORKDIR = $(shell pwd)
BASEDIR = $(WORKDIR)/src/Projects
NAME = dicewars

.PHONY: all build

all: build

build:
	@$(MAKE) -C $(BASEDIR)/GenMap
	@$(MAKE) -C $(BASEDIR)/Strategy

run: build
	@bin/$(DIR)/dicewars$(SUFFIX) -r bin/$(DIR)/referee$(SUFFIX).so -m bin/$(DIR)/genmap$(SUFFIX).so -g bin/$(DIR)/gui$(SUFFIX).so -s bin/$(DIR)/strategytest$(SUFFIX).so -s bin/$(DIR)/strategy$(SUFFIX).so

runtester: build
	@bin/$(DIR)/maptester$(SUFFIX) -m bin/$(DIR)/genmap$(SUFFIX).so

intensemaptester: build
	@bin/$(DIR)/maptester$(SUFFIX) -n 500 -m bin/$(DIR)/genmap$(SUFFIX).so

docker-build: 
	@$(DOCKER) run -v $(WORKDIR):/app $(NAME) make debug=1

docker-run: docker-build
	@$(DOCKER) run -v $(WORKDIR):/app --rm --name $(NAME) -p 5678:5678 $(NAME) /app/bin/$(DIR)/dicewars$(SUFFIX) -r /app/bin/$(DIR)/referee$(SUFFIX).so -m /app/bin/$(DIR)/genmap$(SUFFIX).so -g /app/bin/$(DIR)/gui$(SUFFIX).so -s /app/bin/$(DIR)/strategytest$(SUFFIX).so -s /app/bin/$(DIR)/strategy$(SUFFIX).so

docker-runtester: docker-build
	@$(DOCKER) run -v $(WORKDIR):/app --rm --name $(NAME) -p 5678:5678 $(NAME) /app/bin/$(DIR)/maptester$(SUFFIX) -n 100 -m /app/bin/$(DIR)/genmap$(SUFFIX).so
