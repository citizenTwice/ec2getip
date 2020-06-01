
AWS_INC_DIR = $(HOME)/programs/aws_sdk/include/
AWS_LIB_DIR = $(HOME)/programs/aws_sdk/lib/

LIBS = -laws-cpp-sdk-core -laws-cpp-sdk-ec2 -lssl -lstdc++ -laws-cpp-sdk-core -laws-cpp-sdk-ec2 -lcurl -lcrypto -lpthread

# adding SDK dirs as dependencies, with now timestamp checks, 
# see 'order-only-prerequisites' https://www.gnu.org/software/make/manual/html_node/Prerequisite-Types.html 
all: | $(AWS_INC_DIR) $(AWS_LIB_DIR) ec2getip 
	@echo "Ready"

ec2getip:	ec2getip.cpp credentials.h citizentwice_str.h
	g++ -std=gnu++11 -I$(AWS_INC_DIR) -L$(AWS_LIB_DIR) ec2getip.cpp $(LIBS) -o ec2getip
	
$(AWS_INC_DIR):
	$(error "Can't find $(AWS_INC_DIR) - please edit Makefile to configure the location of AWS SDK")

$(AWS_LIB_DIR):
	$(error "Can't find $(AWS_LIB_DIR) - please edit Makefile to configure the location of AWS SDK")

.PHONY:	all
