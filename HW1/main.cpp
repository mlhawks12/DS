// main.cpp - a very simple seating chart gnerator for a fictitious airline
//
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <sstream>
// a constamt for genrating seat aisle labels
const std::string ALPHABET("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

// return the row and column from a seat request
//
// Note: This code assumes that the seat chart is stored in a 2D vector or
//       array. If you are using a differnt structure for storing seat
//       information, it won't help.
//
// arguments:
// seat_request - a string of row and colummn, eg. 1A
// total_rows - the number of rows in the seating chart
// total_rows - the number of columns in the seating chart, including aisles
// seat_row, seat_col - the seat row and column. If the seat_request is
//                      formatted properly, on return these variables will
//                      contain the row and column of the seat (0-indexed)
//                      
//
// returns;
// true if the seat request contains a valid seat. false otherwise
//
// a seat is valid if its row number is >= 1 and <= the number of rows and
// its column falls in the alphabet in the range of columns.
bool GetSeatRowCol(const std::string& seat_request,
		   unsigned int total_rows,
		   unsigned int total_columns,
		   int& seat_row, std::size_t& seat_col) {
  
  std::string row_str;
  std::string col_str;
  for (unsigned int i = 0; i < seat_request.size(); ++i) {
    if (seat_request[i] >= '0' && seat_request[i] <= '9')
      row_str += seat_request[i];
    else
      col_str += seat_request[i];	
  }
  seat_row = atoi(row_str.c_str()) - 1;
  if (seat_row < 0 || seat_row > (int) total_rows - 1) {
    return false;
  }

  seat_col = ALPHABET.find(col_str);
  if (seat_col == std::string::npos || seat_col > total_columns - 1) {
    return false;
  }

  return true;
}

/*
arguments:
        id - a passenger id that you want to see if it in the seat_chart
        seat_chart- 2D vector that holds the seating chart for the plane
returns:
        false if id is in seat_chart
        true otherwise
*/
bool check_if_in(std::string id, const std::vector<std::vector<std::string> >& seat_chart)
{
    for(int x = 0; x<seat_chart.size(); x++)
    {
        for(int y = 1; y<seat_chart[0].size(); y++)
        {
            if(id == seat_chart[x][y])
            {
                return false;
            }
        }
    }
    return true;
}

//prints the seating chart in the correct format when the PRT command is found Passenger 2D vector
void print_grid(const std::vector< std::vector<std::string> >& chart, std::string flight_num)
{
    std::cout<<"Seating Chart for Flight "<<flight_num<<std::endl;
    std::cout<<"\t";
    for(int x = 0; x<chart[0].size()-1; x++)
    {
        std::cout<<ALPHABET[x]<< "    "; //prints the column letters
    }
    std::cout<<"Seating Class"<<std::endl;
    for(int x = 0; x<chart.size(); x++)
    {
        std::cout<< x+1 << "\t"; // prints the row numbers
        for(int y = 1; y<chart[0].size(); y++)
        {
            std::cout<<chart[x][y]<<" "; //prints the contents of each seat and aisle
        }
        std::cout<<chart[x][0]<<std::endl; //prints the class designation for each row
    }
}
/*
Purpose:
        The function enters passenger ids into seat_chart when a passenger request Coach or when a passenger requests an occupied seat in coach
arguments:
        pass- A vector of strings that contains the current passenger info
        seat_chart- 2D vector containing the seating chart with all seats and aisles
returns:
        void
*/
void C_enter(const std::vector<std::string>& pass,std::vector<std::vector<std::string> >& seat_chart)
{   //This nested for loop takes care of filling the window and ailse seats
    for(int x = 0; x<seat_chart.size(); x++)
    {
        for(int y = 2; y<seat_chart[0].size()-1; y++)
        {
            //if the left most window seat is not taken and the row is Coach, add the passenger id to the seat_chart
            if( seat_chart[x][y-1] == "...." && y == 2   && seat_chart[x][0] == "C")
               {
                   seat_chart[x][y-1] = pass[1];
                   std::cout<<"Passenger " << pass[1] << " was given seat " << x + 1 << ALPHABET[y-2]<<"."<<std::endl;
                   return;
               }
            //if the ailse seat to the left of the ailse is not take and it is Coach, add passenger id to the seat_chart
            else if( (seat_chart[x][y-1] == "...." && seat_chart[x][y] == "!!!!") && seat_chart[x][0] == "C")
               {
                   seat_chart[x][y-1] = pass[1];
                   std::cout<<"Passenger " << pass[1] << " was given seat " << x + 1 << ALPHABET[y-2]<<"."<<std::endl;
                   return;
               }
            //if the ailse seat to the right of the ailse is not take and it is Coach, add passenger id to the seat_chart
            else if( (seat_chart[x][y+1] == "...." && seat_chart[x][y] == "!!!!" ) && seat_chart[x][0] == "C")
            {
                seat_chart[x][y+1] = pass[1];
                std::cout<<"Passenger " << pass[1] << " was given seat " << x + 1 << ALPHABET[y]<<"."<<std::endl;
                return;
            }
            //if the right most window seat is not taken and the row is Coach, add the passenger id to the seat_chart
               if(y == seat_chart[0].size()-2 && seat_chart[x][0] == "C" && seat_chart[x][y+1] == "....")
               {
                   seat_chart[x][y+1] = pass[1];
                   std::cout<<"Passenger " << pass[1] << " was given seat " << x + 1 << ALPHABET[y]<<"."<<std::endl;
                   return;

               }
        }
    }
    //This nested for loop fills the middle seats after all ailse and window seats are full
    for(int x = 0; x<seat_chart.size(); x++)
    {
        for(int y = 2; y<seat_chart[0].size(); y++)
        {
                if (seat_chart[x][y-1] == "...." && seat_chart[x][0] == "C")
                    {
                        seat_chart[x][y-1] = pass[1];
                        std::cout<<"Passenger " << pass[1] << " was given seat " << x + 1 << ALPHABET[y-2]<<"."<<std::endl;
                        return;
                    }
        }
    }
    return;
}

/*
 Purpose:
        The function enters passenger ids into seat_chart when a passenger request First or Business class or when a passenger requests an occupied seat in in these classes
Arguments:
        pass- A vector of strings that contains the current passenger info
        seat_chart- 2D vector containing the seating chart with all seats and aisles
return:
        void
 */
void B_or_F_enter(std::vector<std::string> pass,std::vector<std::vector<std::string> >& seat_chart)
{
    int seat_r;
    std::size_t seat_c;
    //This is to check if the person requested a seat an occuppied seat or requested a class
    bool fun = GetSeatRowCol(pass[2],seat_chart.size(),seat_chart[0].size(),seat_r,seat_c);
    if(fun == 1)//if the person requested a specific seat, but it was occupied, it goes through this code
    {
        for(int x = 0; x<seat_chart.size();x++)
        {
            for(int y = 1; y<seat_chart[0].size();y++)
            {
                //if the seat is empty and the class of the passenger requested his seat to be in is equal to the row it is currently checking and if the seat is not an ailse
                if(seat_chart[x][y] == "...." && seat_chart[seat_r][0] == seat_chart[x][0] && seat_chart[x][y] !="!!!!")
                {
                    seat_chart[x][y] = pass[1];
                    std::cout<<"Passenger " << pass[1] << " was given seat " << x + 1 << ALPHABET[y-1]<<"."<<std::endl;
                    return;
                }
            }
        }
    }
    //If person requested a specific class, it will go through this code
    else
    {
        for(int x = 0; x<seat_chart.size();x++)
        {
            for(int y = 1; y<seat_chart[0].size();y++)
            {
                //if the seat is empty and the current row is equal to the requested section and it is not an ailse
                if(seat_chart[x][y] == "...." && seat_chart[x][0] == pass[2] && seat_chart[x][y] !="!!!!")
                {
                    seat_chart[x][y] = pass[1];
                    std::cout<<"Passenger " << pass[1] << " was given seat " << x + 1 << ALPHABET[y-1]<<"."<<std::endl;
                    return;
                }
            }
        }
    }
}
/*
 Purpose:
        Is called when the passenger request the SRN command which is when he request a specific class or when the SSR function is called but no the seat requested was occupied
        Calls the C_enter function and the B_F_enter functions to figure out where to put the passenger
        Also keeps track of the passengers that are frequent flyiers
Arguments:
        pass- A vector of strings that contains the current passenger info
        seat_chart- 2D vector containing the seating chart with all seats and aisles
        yes_vec- contains the frequent flyier passengers
returns:
        void
 */
void SRN(const std::vector<std::string>& pass,std::vector<std::vector<std::string> >& seat_chart,std::vector<std::vector<std::string> >& yes_vec)
{
    //adds the frequent flyier to the yes_vec
    if(pass[3] == "Y")
    {
        yes_vec.push_back(pass);
    }
    //if the passenger requests coach, tries to put them into their seat by calling C_enter
    if(pass[2] == "C")
        C_enter(pass,seat_chart);
    //if the passenger requests First or Business Class, tries to put them into their seat by calling B_F_enter
    else
    {
        B_or_F_enter(pass,seat_chart);
    }
}
/*
Purpose:
        Is called when the passenger request the SRN command which is when he request a specific class or when the SSR function is called but no the seat requested was occupied
        Calls the C_enter function and the B_F_enter functions to figure out where to put the passenger
        Also keeps track of the passengers that are frequent flyiers
Arguments:
        pass- A vector of strings that contains the current passenger info
        seat_chart- 2D vector containing the seating chart with all seats and aisles
        yes_vec- contains the frequent flyier passengers
        seat_row- empty int to send to GetSeatRowCol to get the number of row
        seat_col- empty size_t to sent to GetSeatRowCol to get the number of column
 returns:
        void
*/
void SSR(const std::vector<std::string>& pass,std::vector<std::vector<std::string> >& seat_chart,int seat_row, std::size_t seat_col,std::vector<std::vector<std::string> >& yes_vec)
{   //checks if requested seat if valid and changes seat_row and seat_col
    bool fun = GetSeatRowCol(pass[2],seat_chart.size(),seat_chart[0].size(),seat_row,seat_col);
    if(fun == 1)
    {
        //adds the frequent flyiers to the yes_vec
        if(pass[3] == "Y" )
        {
            yes_vec.push_back(pass);
        }
        //if the requested seat if empty, put the person the requested seat
        if(seat_chart[seat_row][seat_col+1] == "....")
        {
            seat_chart[seat_row][seat_col+1] = pass[1];
            std::cout<<"Passenger " << pass[1] << " was given seat " <<pass[2]<<"."<<std::endl;

        }
        //checks where the person requested the occupied seat and sends the passenger to be given a empty seat in the Coach class
        else if(seat_chart[seat_row][0] == "C" && (seat_chart[seat_row][seat_col+1] != "...." && seat_chart[seat_row][seat_col+1] != "!!!!"))
        {
            std::cout<<"Passenger "<< pass[1] << " requested an occupied seat "<< seat_row+1 << ALPHABET[seat_col]<<"."<<std::endl;
            C_enter(pass,seat_chart);
        }
        //checks if the person requested an occupied seat in the First or Business class and sends it to be given an empty seat
        else if((seat_chart[seat_row][seat_col+1] != "...." && seat_chart[seat_row][seat_col+1] != "!!!!") && (seat_chart[seat_row][0] == "F" || seat_chart[seat_row][0] == "B"))
        {
            std::cout<<"Passenger "<< pass[1] << " requested an occupied seat "<< seat_row+1 << ALPHABET[seat_col]<<"."<<std::endl;
            B_or_F_enter(pass,seat_chart);
        }
        //if the person requested a invalid seat in First or Business class, then send the passenger to B_F_enter to find a empty seat
        else if(seat_chart[seat_row][0] == "F" || seat_chart[seat_row][0] == "B")
        {
            std::cout<<"Passenger "<< pass[1] << " requested an invalid seat "<< seat_row+1 << ALPHABET[seat_col]<<"."<<std::endl;
            B_or_F_enter(pass,seat_chart);
        }
        //if the person requested a invalid seat in Coach, then send the passenger to C_enter to find a empty seat
        else if(seat_chart[seat_row][0] == "C")
        {
             std::cout<<"Passenger "<< pass[1] << " requested an invalid seat "<< seat_row+1 << ALPHABET[seat_col]<<"."<<std::endl;
             C_enter(pass,seat_chart);
        }
    }
    //if the person requested an invalid row number
    else
        std::cout<<"Passenger "<< pass[1] << " requested an invalid seat "<< pass[2]<<"."<<std::endl;
}
/*
Purpose:
        To cancel a passengers seat request 
Arguments:
        s- The passenger id that is to be canceled
        seat_chart- 2D vector containing the seating chart with all seats and aisles
returns:
        void
*/
void CXL(const std::string& s, std::vector<std::vector<std::string> >& seat_chart)
{
    for(int x = 0; x<seat_chart.size();x++)
    {
        for(int y = 1; y<seat_chart[0].size();y++)
        {
            if(s == seat_chart[x][y])
            {
                seat_chart[x][y] = "....";
                return;
            }
        }
    }
}
/*
Purpose:
        To check what is in the upg_vec
Arguments:
        id- the passenger id that you want to check is in the upg_vec
        upg_vec- people who are scheduled to be upgraded
returns:
        false if id is in the vector
        true otherwise
*/
bool upg_vec_contains(std::string id, std::vector<std::vector<std::string> > upg_vec)
{
    for(int x = 0; x<upg_vec.size(); x++)
    {
            if(id == upg_vec[x][1])
            {
                return false;
            }
    }
    return true;
}
/*
Purpose:
        To check what is in the vec_cxl
Arguments:
        id- the passenger id that you want to check is in the cxl_vec
        cxl_vec- people requested to be canceled
returns:
        false if id is in the vector
        true otherwise
*/
bool check_in_cxl(std::string id, std::vector<std::string> cxl_vec)
{
    for(int x = 0; x<cxl_vec.size(); x++)
    {
            if(id == cxl_vec[x])
            {
                return false;
            }
    }
    return true;
}
/*
Purpose:
        To add passengers to the upg_vec
Arguments:
        seat_chart- 2D vector containing the seating chart with all seats and aisles
        cxl_vec- 2D vecotr of passengers that requested to be canceled
        upg_vec- stores all passengers that are scheduled to be upgraded
        yes_vec- 2D vector that stores all frequent flyiers
returns:
        void
*/
void UPL(std::vector<std::vector<std::string> >& seat_chart, const std::vector<std::string>& cxl_vec, std::vector<std::vector<std::string> >& upg_vec, const std::vector<std::vector<std::string> >& yes_vec)
{
    if(cxl_vec.size() > 0)
    {
    for(int x = 0; x<cxl_vec.size(); x++)
        {
            for(int y = 0; y<yes_vec.size();y++)
            {
            if(check_if_in(yes_vec[y][1], seat_chart) && yes_vec[y][1] != cxl_vec[x] && upg_vec_contains(yes_vec[y][1], upg_vec))
                {
                    upg_vec.push_back(yes_vec[y]);
                    break;
                }
            }
        }
    }
    else
        {
        for(int y = 0; y<yes_vec.size(); y++)
        {
            if(check_if_in(yes_vec[y][1], seat_chart) && upg_vec_contains(yes_vec[y][1], upg_vec))
            {
                upg_vec.push_back(yes_vec[y]);
            }
        }
            
    }
}

int main(int argc, char* argv[]) {
    //checks if the number of argument files is correct
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " main-file\n";
        return 1;
    }
    std::ifstream seats(argv[1]);//takes in the first text file
    //checks if the file can be opened
    if (!seats.good()) {
        std::cerr << "Can not open the seats file " << argv[1] << "\n";
        return 1;
    }
    std::ifstream passengers(argv[2]);//takes in the second txt file
    //checks if the file is able to open
    if (!passengers.good()) {
        std::cerr << "Can not open the seats file " << argv[2] << "\n";
        return 1;
    }
    
    std::vector<std::vector<std::string> > seat_chart;//intialize seating chart
    std::string str_1;
    //reads the first argument file
    while (std::getline(seats, str_1))
    {
        std::istringstream iss (str_1);//used to split the line at spaces
        std::vector<std::string> dumb;
        std::string s;
        while(iss>>s)//to add each part of the split string to a dummy vector to be added to the 2D vector
        {
            //changes the . ot .... and ! to !!!!
            if(s == ".")
                s = "....";
            if(s == "!")
                s = "!!!!";
            dumb.push_back(s);
        }
        seat_chart.push_back(dumb);
    }
   
    std::vector<std::vector<std::string> > pass_in;
    std::string str_2;
    //reads the second argument file
    while (std::getline(passengers, str_2))
    {
        std::istringstream iss (str_2);//splits line at spaces
        std::vector<std::string> dumb;
        std::string s;
        while(iss>>s)//adds the split strings to a dummy vector to be added to the 2D vectot
        {
            dumb.push_back(s);
        }
        pass_in.push_back(dumb);
    }
    int seat_row=0;
    std::size_t seat_col =0;
    std::vector<std::string> cxl_vec;
    std::vector<std::vector<std::string> > yes_vec;
    std::vector<std::vector<std::string> >upg_vec;
    //goes through the passenger vector and calls the correct function based on the command the passenger has
    for(int i = 1; i<pass_in.size(); i++)
    {
        if(pass_in[i][0] == "SRN")
            SRN(pass_in[i],seat_chart,yes_vec);
        if(pass_in[i][0] == "SSR")
            SSR(pass_in[i],seat_chart,seat_row,seat_col,yes_vec);
        if(pass_in[i][0] == "PRT")
            print_grid(seat_chart,pass_in[0][0]);
        if(pass_in[i][0] == "UPL")
        {
            UPL(seat_chart, cxl_vec,upg_vec,yes_vec);
            std::cout<<"The following passengers are scheduled to be upgraded to first class:"<<std::endl;
            for(int a = 0; a<upg_vec.size();a++)
            {
                //To check if the person that was schedule to be upgraded was not canceled and then print it
                if(check_in_cxl(upg_vec[a][1], cxl_vec) && upg_vec[a][2] == "B")
                {
                    std::cout<<upg_vec[a][1]<<std::endl;
                }
            }
            std::cout<<"The following passengers are scheduled to be upgraded to business class:"<<std::endl;
            for(int a = 0; a<upg_vec.size();a++)
            {
                //To check if the person that was schedule to be upgraded was not canceled and then print it
                if(check_in_cxl(upg_vec[a][1], cxl_vec) && upg_vec[a][2] == "C")
                {
                    std::cout<<upg_vec[a][1]<<std::endl;
                }
            }
        }
        int check = 0;//makes sure to only upgrade the passenger once
        if(pass_in[i][0] == "UPP")
        {
            for(int x = 1; x<pass_in.size();x++)
            {
                for(int y = 0; y<upg_vec.size();y++)
                {
                    if(pass_in[x][0] == "SRN" || pass_in[x][0] == "SSR")
                    {
                    if(upg_vec[y][2] == "C" && check == 0)
                    {
                        upg_vec[y][2] = "B";//upgrades the person to Business
                        check = 1;
                    }
                    else if(upg_vec[y][2] == "B" && check == 0)
                    {
                        upg_vec[y][2] = "F";//upgrades the person to First class
                        check = 1;
                    }
                std::vector<std::vector<std::string> > dumb_2;//dummy vector to satisfy a parameter not needed any more
                if(pass_in[x][1] == upg_vec[y][1] && check_in_cxl(upg_vec[y][1], cxl_vec) && check_if_in(upg_vec[y][1],seat_chart))
                {
                    
                    SRN(upg_vec[y],seat_chart,dumb_2);//calls SRN to change seat_chart
                    if(upg_vec[y][2] == "F")
                        std::cout<<"Passenger "<<upg_vec[y][1] <<" was upgraded to the first class section."<<std::endl;
                    if(upg_vec[y][2] == "B")
                        std::cout<<"Passenger "<<upg_vec[y][1] <<" was upgraded to the Business class section."<<std::endl;
                }
                }
                check = 0;
                }
            }
        }
        if(pass_in[i][0] == "CXL")
        {
            CXL(pass_in[i][1], seat_chart);
            cxl_vec.push_back(pass_in[i][1]);
        }
    }
    return 0;
}









