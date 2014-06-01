/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include <stdio.h> 
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <linux/input.h>



int main(int argc, char **argv) { 
	fd_set readstruct;
	struct input_event event;
	int maxfd = -1;
	int fd = -1;
	int c=-1;
	char* inputdev=NULL;
	static char* VERSION="0.3";
	while ((c=getopt(argc,argv,"f:")) != -1) {
		if (c=='f') {
			inputdev=optarg;
		}
	}
	if (inputdev == NULL) {
		fprintf(stdout,"===============================================================\n");
		fprintf(stdout,"joybutt %s - 2009-2014 by Tony Bussieres.\n",VERSION);
		fprintf(stdout,"This program reads key from input event device.\n");
		fprintf(stdout,"GPLv3. get the license at http://www.gnu.org/copyleft/gpl.html\n");
		fprintf(stdout,"===============================================================\n");
		fprintf(stdout,"you need to pass the event device file -f /dev/input/eventXX\n");
		return -1;
	}
	
	fprintf(stdout,"Trying to open input : %s\n",inputdev);
	fd = open(inputdev, O_RDONLY );
	if (fd < 0) {         
		fprintf(stderr,"Failed to open input : %s\n",inputdev);
		return -1;
	}
	fprintf(stdout,"Open successful, reading key event\n");
	FD_ZERO(&readstruct);
	FD_SET( fd, &readstruct );
	maxfd = fd+1;
	while(1)
	{
	  struct timeval timeout;
	  timeout.tv_sec=10;
	  timeout.tv_usec=0;
	  
	  int ret = select( maxfd, &readstruct, 0, 0, &timeout );
	  if( ret == 0 )
	  {
		  timeout.tv_sec=10;
		  timeout.tv_usec=0;
	  }
	  else if( ret > 0 )
	  { 
		  int r = read( fd, &event, sizeof(struct input_event));
		  if (r == sizeof(struct input_event)) {
			  if (event.type == EV_KEY) { // just key event
				  fprintf(stdout,"%d\t%d\n",event.value,event.code);
				  fflush(stdout);
			  }
		  } else {
			  fprintf(stderr,"Unable to read from device!\n");
			  close(fd);
			  return -1;
		  }
	  }
	  else 
	  {  
	    break;
	  }
	  FD_SET( fd, &readstruct );
	}
	close(fd);
	return 0; 
} 
