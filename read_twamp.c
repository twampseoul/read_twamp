#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

typedef struct simInput{
	int bandwidth;
	int delay;
	double loss;
} simInput_t;

int main(){
	// Compile using this command:
	// gcc read_twamp.c -o read_twamp -l sqlite3 -fno-stack-protector
	// BEGIN database preparation
	int retval;
    int q_cnt = 5,q_size = 150,ind = 0;
    char **queries = malloc(sizeof(char) * q_cnt * q_size);
    
    sqlite3_stmt *stmt;
    sqlite3 *handle;
    retval = sqlite3_open("ns3.sqlite3",&handle);
    if(retval)
    {
        printf("Database connection failed\n");
        return -1;
    }
    printf("Connection successful\n");
	// END database preparation
	
	// BEGIN read file
	simInput_t inpData[2000];
	
	FILE *f;FILE *p;
	int i = 0;
	
	int d_delay;
	double d_loss;
	f = fopen("twamp.txt","rw");
	
	while(fscanf(f, "%d, %d, %lf", &inpData[i].bandwidth, &inpData[i].delay, &inpData[i].loss) > 1){	
		// BEGIN to_string variables
		char user_bw[] = " --user_bw_down=";
		char user_bw_op[15];
		snprintf(user_bw_op, 15, "%d", inpData[i].bandwidth);
		
		char avg_delay[] = " --avg_delay_dw=";
		char avg_delay_op[15];
		snprintf(avg_delay_op, 15, "%d", inpData[i].delay);
		
		char err[] = " --errRate=";
		char err_op[15];
		snprintf(err_op, 15, "%f", inpData[i].loss);	
		// END to_string variables
		
		// BEGIN query availability of record
		/*
		 * char cek[170] = "SELECT * from skt where ";
		char c_delay[] = " delay=";
		char c_and[] = " and ";
		char c_loss[] = " loss=";  
		strcat(cek, c_delay); 
		strcat(cek, avg_delay_op); 
		strcat(cek, c_and); 
		strcat(cek, c_loss); 
		strcat(cek, err_op); 
		
		retval = sqlite3_prepare_v2(handle,cek,-1,&stmt,0);
		retval = sqlite3_step(stmt);
		* */
		// END query availability of record
		
		// conditional logic checks whether there is a return for 
		// above select query. If no, it means no value for such var
		// and then simulation is executed
		
		//if(retval == SQLITE_DONE){
		if(1){
			// BEGIN simulation execution
			
			char toc[] = " --TypeOfConnection=p --ModeOperation=true --tcp_mem_user=4096,87380,8388608 --tcp_mem_user_wmem=4096,87380,8388608 --tcp_mem_user_rmem=4096,87380,8388608 --tcp_mem_server=4096,87380,8388608 --tcp_mem_server_wmem=4096,87380,8388608 --tcp_mem_server_rmem=4096,87380,8388608 --tcp_cc=reno --SimuTime=13.00 --user_bw_up=1Gbps --ErrorModel=1 --ErrorModel2=1 --errRate2=0.000000 --chan_k_dw=0.100 --delay_pdv_dw=0.000 --chan_k_up=0.100 --avg_delay_up=1.000 --delay_pdv_up=0.000 ";
			char last_quote[] = " \"";
			char mbps[] = "Mbps ";
			char let[700] = "./waf --run \"kupakupa ";
			strcat(let, toc);
			strcat(let, user_bw);
			strcat(let, user_bw_op);
			strcat(let, mbps);
			strcat(let, err);
			strcat(let, err_op);
			strcat(let, avg_delay);
			strcat(let, avg_delay_op);
			strcat(let, last_quote);
			system(let);
			printf("wafcom:: %s\n", let);
			// END simulation execution
			
			// BEGIN reading log
			char throughput_op[10];
			p = popen("cat ~/NS3Project/dce/source/ns-3-dce/files-0/var/log/38886/stdout |grep bits| awk '{printf $7}'", "r");
			
			if(!p){
				printf("Error opening pipe bro.. \n");
				fprintf(stderr, "Error opening pipe.\n");
				return 1;
			}
			
			if(!feof(p)){
				double throughput;
				fscanf(p, "%lf", &throughput);
				snprintf(throughput_op, 50, "%f", throughput);
			}
			// END reading log
			
			// Insert input variable into 'trial' table
			// sqlite> drop table skt;
			// sqlite> CREATE TABLE skt(userbw REAL, delay REAL, loss REAL, throughput REAL);
			char separator[] = "','";
			char ending[] = "')";
			char query[100] = "INSERT INTO skt VALUES('";
			strcat(query, user_bw_op);
			strcat(query, separator);
			strcat(query, avg_delay_op);
			strcat(query, separator);
			strcat(query, err_op);
			strcat(query, separator);
			strcat(query, throughput_op);
			strcat(query, ending);
			queries[ind++] = query;
			printf("\nVar: %s %s %s\n", avg_delay_op, err_op, throughput_op);
			
			retval = sqlite3_exec(handle,queries[ind-1],0,0,0);
		}
		i++;
	}
	printf("\n");
	// END read file

	/*
	 *  --user_bw_down=150Mbps --errRate=0.000010 --avg_delay_dw=50.000
	 *  
	 *  --TypeOfConnection=p --ModeOperation=true --tcp_mem_user=4096,87380,8388608 
	 *  --tcp_mem_user_wmem=4096,87380,8388608 --tcp_mem_user_rmem=4096,87380,8388608 
	 *  --tcp_mem_server=4096,87380,8388608 --tcp_mem_server_wmem=4096,87380,8388608 
	 *  --tcp_mem_server_rmem=4096,87380,8388608 --tcp_cc=reno --SimuTime=100.00 
	 *  --user_bw_up=1Gbps --ErrorModel=1  
	 *  --ErrorModel2=1 --errRate2=0.000000 --chan_k_dw=0.100  
	 *  --delay_pdv_dw=0.000 --chan_k_up=0.100 --avg_delay_up=1.000 --delay_pdv_up=0.000
	 * */
	

	fclose (f);
	return 0;
}
