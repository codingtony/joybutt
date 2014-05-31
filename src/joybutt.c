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
#include <linux/joystick.h>
#include <ctype.h>



int main(int argc, char **argv) { 
	fd_set readstruct;
	struct js_event event;
	int maxfd = -1;
	int fd = -1;
	int c=-1;
	char* joydev=NULL;
	static char* VERSION="0.2";
	while ((c=getopt(argc,argv,"f:")) != -1) {
		if (c=='f') {
			joydev=optarg;
		}
	}
	if (joydev == NULL) {
		fprintf(stdout,"===============================================================\n");
		fprintf(stdout,"joybutt %s - 2009-2014 by Tony Bussieres.\n",VERSION);
		fprintf(stdout,"GPLv3. get the license at http://www.gnu.org/copyleft/gpl.html\n");
		fprintf(stdout,"===============================================================\n");
		fprintf(stdout,"you need to pass the joystick device file -f /dev/input/jsXXX\n");
		return -1;
	}
	
	fprintf(stdout,"Trying to open joystick : %s\n",joydev);
	fd = open(joydev, O_RDONLY );
	if (fd < 0) {         
		fprintf(stderr,"Failed to open joystick : %s\n",joydev);
		return -1;
	}
	fprintf(stdout,"Open successful, reading joystick button event\n");
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
		  int r = read( fd, &event, sizeof(struct js_event));
		  if (r == sizeof(struct js_event)) {
			  if (event.type == JS_EVENT_BUTTON) { // just capture button
				  fprintf(stdout,"%d\t%d\n",event.value,event.number);
				  fflush(stdout);
			  }
		  } else {
			  fprintf(stderr,"Unable to read from joystick device!\n");
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
