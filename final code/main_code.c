// By Shahrukh khan
// khanskid111@gmail.com

#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>

#define LEFT 0x88
#define RIGHT 0x22
#define FORWARD 0xAA
#define BACKWARD 0x55
#define STOP 0x00
#define ON 0xAA;
#define OFF 0x00;
#define ROTATE 0x99;

// Function declaration
int calc_dev();
void left();
void right();
void push();
void wait();
void rotate();
void forward(int dev);
void update_pos();
char calc_dirn();
void move_bot(char cd,char pd);
void reset_traverse();
void def_path(int r,int c);
void init_variables();

int x_l[4]={4,0};
int y_l[4]={3,0};
int r=0,c=0;
int x,y;      // base coordinates
char p_dirn,c_dirn;
int c_d=0;  //count destinations picked
char traverse_path_points[25][2];
char traverse_path_check[8][8];
char checkpost[9][9];
int steps=0,reach=0;
int n=4,m=4;
char new_p[25][2];
char p_s=20;

int traversed_pnt_np=1;

void main()
{
 DDRD=0xFF;
 DDRA=0x00;
 DDRC=0xFF;
 DDRB=0xFF;
 TCCR1A=0xA1;
 TCCR1B=0x09;
 
int dev;
int kp=15,pwm;
int t=0;

//check_search variables
int search_flag=1;

x=x_l[c_d];
y=y_l[c_d];

//initialise variables
int init_origin=0;   // check to see to reach origin first time
p_dirn='N';
c_dirn='N';
r=0;c=0;
PORTB=0x00;
reset_traverse();

for(int i=0;i<8;i++)
{
	for(int j=0;j<8;j++)
	{
		checkpost[i][j]=0;
	}
}
 while(1)
   {  OCR1AL=195;
      OCR1BL=195;
      dev=calc_dev();
      if(dev<20)
	  {
	   forward(dev);
	  }   		
			
	else if(dev==25)
	  { t+=1;    //count no of T
        if(init_origin==0)
		{   r=0;c=0;
	        init_origin=1;
			PORTB=0x00;
			def_path(r,c);
		}
		else {update_pos();}   //update pos of bot
		
		if(r==x && c==y)
		{   wait();
		    c_d+=1;
			search_flag=1;
			traversed_pnt_np=1;
			x=x_l[c_d];
            y=y_l[c_d];
			reset_traverse();
			
		}
		
		if(search_flag==1)
		{   PORTB=0x00;
			def_path(r,c);
			search_flag=0;
		}
		c_dirn=calc_dirn();
		move_bot(p_dirn,c_dirn);
		p_dirn=c_dirn;
      }
	}
 }
// END OF MAIN LOOP 

void reset_traverse()
{
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			traverse_path_check[i][j]=0;
		}
	}
}

int calc_dev()
{
 int s[8],sum=0,count=0;

     if(bit_is_set(PINA,0))
		     {s[0]=0;
			 }
		 else if(bit_is_clear(PINA,0))
		     {s[0]=-4;
			 }
	 if(bit_is_set(PINA,1))
		     {s[1]=0;
			 }
		 else if(bit_is_clear(PINA,1))
		     {s[1]=-3;
			 }
     if(bit_is_set(PINA,2))
		     {s[2]=0;
			 }
		 else if(bit_is_clear(PINA,2))
		     {s[2]=-2;
			 }
     if(bit_is_set(PINA,3))
		     {s[3]=0;
			 }
		 else if(bit_is_clear(PINA,3))
		     {s[3]=-1;
			 }
     
	 if(bit_is_set(PINA,4))
		     {s[4]=0;
			 }
		 else if(bit_is_clear(PINA,4))
		     {s[4]=1;
			 }
     if(bit_is_set(PINA,5))
		     {s[5]=0;
			 }
		 else if(bit_is_clear(PINA,5))
		     {s[5]=2;
			 }
	 if(bit_is_set(PINA,6))
		     {s[6]=0;
			 }
		 else if(bit_is_clear(PINA,6))
		     {s[6]=3;
			 }
	 if(bit_is_set(PINA,7))
		     {s[7]=0;
			 }
		 else if(bit_is_clear(PINA,7))
		     {s[7]= 4;
			 }		 
   
	 for(int i=0;i<8;i++)
	    {sum+=s[i];
		 if(s[i]==0)
		    {count+=1;
			}
        }
    if(  (bit_is_clear(PINA,1)) &&( bit_is_clear(PINA,2)) && (bit_is_clear(PINA,3)) && (bit_is_clear(PINA,4)) && (bit_is_clear(PINA,5)) && (bit_is_clear(PINA,6)) ) 
	   {sum=25;
	   }
    else if( (bit_is_clear(PINA,0)) && (bit_is_clear(PINA,1)) &&(bit_is_clear(PINA,2)) ) 
	   {sum=25;  //left
	   }
    else if( (bit_is_clear(PINA,4)) &&(bit_is_clear(PINA,5)) && (bit_is_clear(PINA,6)) ) 
	   {sum=25;  //right
	   }
	
	return(sum);
}

void left()
{
  PORTB=LEFT;
 _delay_ms(1300);
}
void right()
{
  PORTB=RIGHT;  //right
  _delay_ms(1300);
		
}
void push()
{ 
  
  PORTB=ON;
  _delay_ms(1000);
}
void wait()
{ 
  PORTB=0x00;
  PORTC=0xAA;
  _delay_ms(2000);
  PORTC=0x00;
}
void rotate()
{
  PORTB=ROTATE;
  _delay_ms(1500);
}
void forward(int dev)
{
 if(dev==0) 
	   {PORTB=FORWARD;
	    	   }
	   else if (dev>0 && dev <10)
	   {
	    PORTB=RIGHT;  //right
	   }
	 	else if(dev<0)
		{
	     PORTB=LEFT;
		}		
}

void update_pos()
{
	
 if(p_dirn=='N')
 {
	 r+=1;
 }
  else if(p_dirn=='S')
  {   
	r-=1;  
  }
  else if(p_dirn=='E')
  {
	  c+=1;
  }
  else if(p_dirn=='W')
  {
	  c-=1;
  }

}

char calc_dirn()
{  int dx,dy;
   char dirn;
   int xc,yc;
    update: xc=new_p[traversed_pnt_np][0];yc=new_p[traversed_pnt_np][1];
     dx=xc-r;  dy=yc-c;
	
	if(dx !=0)
	{
		dirn=dx>0 ?'N' :'S';
	}
	else if(dy!=0)
	{
		dirn=dy>0 ?'E' :'W';
	}
	else{
		traversed_pnt_np+=1;
		goto update;
	}
	return (dirn);
}	

void move_bot(char pd,char cd)
{
	
	if(pd==cd)
	{
		push();
	}
	else if( (pd=='E' && cd =='W') || (pd=='N' && cd =='S') || (pd=='S' && cd =='N') || (pd=='W' && cd =='E')  ) 
	{
		rotate();
	}
	else if( (pd=='E' && cd =='S') || (pd=='N' && cd =='E') || (pd=='S' && cd =='W') || (pd=='W' && cd =='N')  ) 
	{
		right();
	}
	else if( (pd=='E' && cd =='N') || (pd=='N' && cd =='W') || (pd=='S' && cd =='E') || (pd=='W' && cd =='S')  ) 
	{
		left();
	}
	
	
}


char check_valid(int nr,int nc)     ///  nr,nc  ----means next row and next column
{   char valid_bound=0,valid_trace=0,valid_reach=0,valid_node=0,valid_length;//check boundary condn
    
	valid_bound=(nr<=n && nc<=m && nr>=0 && nc>=0)? 1 :0;
	valid_reach=(nr!=x && nc!=y)? 1 :0;
	valid_trace=(traverse_path_check[nr][nc]==0)?1:0;
	valid_node =(checkpost[nr][nc]==0)?1:0;
	valid_length=(steps < p_s ) ? 1:0;
	/*if(nr==x && nc==y)
	{
		reach=1;
	}*/
		
	if(valid_bound && valid_trace  && valid_node  && valid_length)
	{
		return (1);
	}
	else{
		return(0);
	}
	
}

void def_path(int r,int c)
{   char f1=0,f2=0,f3=0,f4=0;
	
	traverse_path_points[steps][0]=r;    ///update travel path 
	traverse_path_points[steps][1]=c;
	traverse_path_check[r][c]=1;
    
	
	if(r==x && c ==y)
	{
		reach=1;
	}
	else
	{
	f1=check_valid(r+1,c);   //check for valid move
	f2=check_valid(r,c+1);
	f3=check_valid(r-1,c);
	f4=check_valid(r,c-1);
	}
	if(f1==1)
	{   
		steps+=1;
		def_path(r+1,c);
		steps-=1;
	}
	if(f2==1)
	{
		steps+=1;
		def_path(r,c+1);
		steps-=1;
	}
	if(f3==1)
	{
		steps+=1;
		def_path(r-1,c);
		steps-=1;
	}
	if(f4==1)
	{
		steps+=1;
		def_path(r,c-1);
		steps-=1;
	}
	//new_p[10][0]=5;
	if(f1==0 && f2==0 && f3 ==0 && f4 ==0)
	{
		if(reach==1)
	    {    reach=0;
		     if(steps<p_s){
				p_s=steps;
			for(int i=0;i<=steps;i++)
			{   
				new_p[i][0]=traverse_path_points[i][0];
				new_p[i][1]=traverse_path_points[i][1];
			}
		}                   }
		
    }
	traverse_path_check[r][c]=0;
} 
