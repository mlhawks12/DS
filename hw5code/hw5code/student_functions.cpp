#include <fstream>
#include <iostream>
#include <string>
#include "SongLibrary.h"

//////////////////DO NOT CHANGE THIS CODE////////////////////////

//Construct a Song object with values
Song::Song(const std::string& title, const std::string& artist, 
     const std::string& album, unsigned int year){
	this->title = title;
	this->artist = artist;
	this->album = album;
	this->year = year;
	used = false;
	this->song_group_ptr = NULL;
}

//Song equality doesn't depend on pointers or being used
bool operator== (const Song& s1, const Song& s2){
	return (s1.getYear() == s2.getYear()) && (s1.getTitle() == s2.getTitle())
          && (s1.getAlbum() == s2.getAlbum()) && (s1.getArtist() == s2.getArtist());
}

//Modifier for group pointer
void Song::updateSongGroupPointer(SongGroupNode* ptr){
	if(ptr){
		//Check that ptr is actually for this Song.
		assert(ptr->m_song->value == *this); 
		song_group_ptr = ptr;
	}
	else{
		song_group_ptr = NULL;
	}
}

//Make output easier by overload operator<<
std::ostream& operator<< (std::ostream& ostr, const Song& song){
	if(!song.getSongGroupPointer()){
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
	       << song.getAlbum() << "\" (" << song.getYear() << ")" << std::endl;
	}
	else{
		ostr << "\"" << song.getTitle() << "\" by " << song.getArtist() << " on \"" 
		<< song.getAlbum() << "\" (" << song.getYear() << ")" << " in Song Group "
		<< song.getSongGroupPointer()->id << std::endl;
	}
	return ostr;
}

/////////////////////////YOUR CODE STARTS HERE/////////////////////////////////


//////////////////////////////GROUP FUNCTIONS/////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are searching for
BEHAVIOR:
None
RETURN:
NULL if no list matches the ID, otherwise returns the GroupListNode* containing
the head of the list with ID id.
*/
GroupListNode* GroupExists(GroupListNode* song_groups, GroupID id){
    GroupListNode* search = song_groups;
    if(search == NULL || search->value == NULL)
    {
        return NULL;
    }
    while(search != NULL)
    {
        if(search->value->id == id)
        {
            return search;
        }
        search = search->ptr;
    }
    return NULL;
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-id is the ID of the list we are creating
BEHAVIOR:
Add a new song group to song_groups, give the new song group ID id. Order of
song groups is not guaranteed.
RETURN:
None
*/
void MakeGroup(GroupListNode*& song_groups, GroupID id){
    //Push Group list node to the front
    GroupListNode* group_head = song_groups;
    GroupListNode* gln = new GroupListNode;
    SongGroupNode* sgn = new SongGroupNode;
    gln -> value = sgn;
    gln -> ptr = group_head;
    song_groups = gln;
    sgn -> id = id;
    sgn -> m_song = NULL;
    sgn -> next_song_ptr = NULL;
    sgn -> prev_song_ptr = NULL;
    sgn -> prev_by_year_ptr = NULL;
    sgn -> next_by_artist_ptr = NULL;
}
/*
ARGUMENTS:
-group_head points to the head of a song group
-song is the Song we are checking the group for
BEHAVIOR:
None
RETURN:
If song is found in the group return true. If song is not found,
return false.
*/
bool SongInGroup(SongGroupNode* group_head, const Song& song){
    SongGroupNode* temp = group_head;
    while(temp != NULL)
    {
        if(temp->m_song && (temp->m_song->value == song))
            return true;
        temp = temp->next_song_ptr;
    }
    return false;
}
/*
 ARGUMENTS:
 -list_ptr points to the head GroupListode that you want to remcompute the next_by_artist_ptr
 BEHAVIOR:
 Recompute all next_by_artist_ptr members in the song group pointed to by
 list_ptr.
 RETURN:
 none
 */
void RecomputeNextArtist(GroupListNode* list_ptr)
{
    
    SongGroupNode* head = list_ptr->value;
    while(head && head->next_song_ptr)
    {
        SongGroupNode* next = head->next_song_ptr;
        while(next && next->next_song_ptr)
        {
            if(head->m_song->value.getArtist() == next->m_song->value.getArtist())
            {
                head->next_by_artist_ptr = next;
                next->next_by_artist_ptr = NULL;
                //break;
            }
            next = next->next_song_ptr;
        }
        head = head->next_song_ptr;
        if(!next->next_song_ptr)
        {
            if(head->m_song->value.getArtist() == next->m_song->value.getArtist())
            {
                head->next_by_artist_ptr = next;
                next->next_by_artist_ptr = NULL;
            }
        }
    }
    
}
/*
 ARGUMENTS:
 -library_ptr points to the LibraryNode* containing the Song we want to add.
 -list_ptr is the head of the song group we want to add the song to.
 BEHAVIOR:
 Modify the group pointed to by list_ptr to reflect that the song is in the group.
 This may change the length of the list. Modifies the Song's members as well.
 RETURN:
 None
 */
void AddToGroup(LibraryNode* library_ptr, GroupListNode* list_ptr)
{
    if(!list_ptr->value->m_song)
    {
        list_ptr->value->m_song = library_ptr;
        list_ptr->value->m_song->value.setUsedInGroup(true);
        list_ptr->value->m_song->value.updateSongGroupPointer(list_ptr->value);
    }
    else
    {
        SongGroupNode* tail = list_ptr->value;
        while(tail->next_song_ptr)
        {
            tail = tail->next_song_ptr;
        }
        SongGroupNode* new_song = new SongGroupNode;
        new_song->next_song_ptr = NULL;
        tail->next_song_ptr = new_song;
        new_song->m_song = library_ptr;
        new_song->prev_song_ptr = tail;
        new_song->id = tail->id;
        new_song->m_song->value.setUsedInGroup(true);
        new_song->m_song->value.updateSongGroupPointer(new_song);
        RecomputePrevByYear(list_ptr);
        RecomputeNextArtist(list_ptr);
    }
    
}
/*
ARGUMENTS:
-library_ptr points to the LibraryNode* containing the Song we want to remove from a group.
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Modifies the Song's member variables to reflect that it is not in a group. Modifies the group
so that the Song is not in the group. This function may cause song_groups to be modified as well.
RETURN:
None
*/

void RemoveFromGroup(LibraryNode* library_ptr, GroupListNode* song_groups){
    GroupListNode* cursor;
    if(!library_ptr)
        return;
    if(SongInGroup(song_groups->value, library_ptr->value))
        cursor = song_groups;
    else
    {
        cursor = song_groups->ptr;
        if(cursor->ptr != NULL)
        {
            while(!SongInGroup(cursor->value, library_ptr->value))
            {
                cursor = cursor->ptr;
            }
        }
    }
    //One thing in group
    library_ptr->value.setUsedInGroup(false);
    if(cursor->value->m_song != NULL &&cursor->value->m_song->value == library_ptr->value && cursor->value->prev_song_ptr == NULL && cursor->value->next_song_ptr == NULL)
        {
            cursor->value->m_song = NULL;
    }
    else
    {
        SongGroupNode* search = cursor->value;
        while((search->m_song != library_ptr) && search->next_song_ptr != NULL)
        {
            search = search->next_song_ptr;
        }
        //delete head
        if(search->prev_song_ptr == NULL && search->next_song_ptr!= NULL && search->m_song->value == library_ptr->value)
        {
            cursor->value = search->next_song_ptr;
            search->next_song_ptr->prev_song_ptr = NULL;
            delete search;
            
        }
        //delete tail
        else if(search->next_song_ptr == NULL && search->prev_song_ptr != NULL && search->m_song->value == library_ptr->value)
        {
            SongGroupNode* new_tail = search->prev_song_ptr;
            new_tail->next_song_ptr = NULL;
            delete search;
        }
        //delete in middle
        else if(search->next_song_ptr != NULL && search->prev_song_ptr !=NULL)
        {
            SongGroupNode* prev = search->prev_song_ptr;
            SongGroupNode* next = search->next_song_ptr;
            delete search;
            prev->next_song_ptr = next;
            next->prev_song_ptr = prev;
        }
    RecomputePrevByYear(song_groups);
    RecomputeNextArtist(song_groups);
    }
}
/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr1 contains to the song group head of Group A
-list_ptr2 contains the song group head of Group B
BEHAVIOR:
Modify Group A so that at the end of its songs, the songs of Group B are added in the same
order they were found in Group B. Remove the now-empty Group B.
RETURN:
None
*/
void CombineGroups(GroupListNode*& song_groups, GroupListNode*& list_ptr1, GroupListNode*& list_ptr2){
    GroupListNode* search = song_groups;
    while (search->ptr != list_ptr1) {
        search = search ->ptr;
    }
    SongGroupNode* tail = list_ptr1->value;
    while(tail->next_song_ptr)
        tail = tail->next_song_ptr;
    SongGroupNode* cursor = list_ptr2->value;
    while(cursor->next_song_ptr)
    {
        cursor->id = tail->id;
        LibraryNode* temp = cursor->m_song;
        AddToGroup(temp, list_ptr1);
        cursor = cursor->next_song_ptr;
    }
    RemoveGroup(song_groups,list_ptr2);
}

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
-list_ptr is the head of the song group we want to remove.
BEHAVIOR:
Remove the group from the list of song groups. No Song objects should remain associated with the
group, and the function should modify Song member variables appropriately to reflect this. Any
memory associated with the group must be released.
RETURN:
None
*/
void RemoveGroup(GroupListNode*& song_groups, GroupListNode* list_ptr){
    if(!song_groups|| !list_ptr)
        return;
    GroupListNode* search = song_groups;
    while(search->value)
    {
        LibraryNode* song = list_ptr->value->m_song;
        RemoveFromGroup(song, search);
        search->value = search->value->next_song_ptr;
    }
    GroupListNode* search2 = song_groups;
    while (search2 && search2->ptr)
    {
        if(search2->ptr == list_ptr)
        {
            search2->ptr = search->ptr->ptr;
            delete list_ptr;
        }
        search2 = search2->ptr;
    }
    delete search2;
}

/*
ARGUMENTS:
-list_ptr is the head of the song group we want to add the song to.
BEHAVIOR:
Recompute all prev_by_year_ptr members in the song group pointed to by
list_ptr.
RETURN:
None
*/

void RecomputePrevByYear(GroupListNode* list_ptr){
    if(list_ptr)
        list_ptr->value->prev_by_year_ptr = NULL;
    SongGroupNode* tail = list_ptr->value;
    while(tail->next_song_ptr)
    {
        tail = tail->next_song_ptr;
    }
    SongGroupNode* search = tail;
    while(search->prev_song_ptr)
    {
        SongGroupNode* previous = search->prev_song_ptr;
        while(previous)
        {
            if(!previous->prev_song_ptr)
                previous->prev_by_year_ptr = NULL;
            if(search->m_song->value.getYear() > previous->m_song->value.getYear())
            {
                search->prev_by_year_ptr = previous;
                break;
            }
            search->prev_by_year_ptr = NULL;
            previous = previous->prev_song_ptr;
        }
        if(!search->prev_song_ptr)
            search->prev_by_year_ptr = NULL;
        search = search->prev_song_ptr;
    }
}
////////////////////////////PRINT FUNCTIONS////////////////////////////////////

/*
ARGUMENTS:
-library_head points to the first LibraryNode* of the song library
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the library in the order it was added.
RETURN:
None
*/
void PrintLibrary(LibraryNode* library_head,std::ostream& print_file){
    LibraryNode* temp = library_head;
    int c =0;
    while(temp)
    {
        print_file<< temp->value;
        temp = temp->ptr;
        c++;
    }
    print_file<<"Library contains " << c << " song(s)"<<std::endl;
}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to print
-print_file points to an ostream to print output to
BEHAVIOR:
Print each song in the group in the order it was added.
RETURN:
None
*/
void PrintGroupSequential(SongGroupNode* group_head,std::ostream& print_file){
    SongGroupNode* temp = group_head;
    print_file<<"List ID " << group_head->id<< " contains:" <<std::endl;
    int c = 0;
    bool check = false;
    if(temp->m_song!= NULL)
    {
        c++;
        print_file<<temp->m_song->value;
        check = true;
    }
    while(temp->next_song_ptr != NULL)
    {
        if(check)
            temp = temp->next_song_ptr;
        print_file<<temp->m_song->value;
        if(!check)
            temp = temp->next_song_ptr;
        c++;
    }
    print_file<<c<< " song(s)"<<std::endl;
}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the prev_by_year_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupRewind(SongGroupNode* group_ptr,std::ostream& print_file){
    SongGroupNode* search = group_ptr;
    int count = 1;
    print_file<<"Rewind list for " << group_ptr->m_song->value.getYear()<<std::endl;
    if(!group_ptr->prev_by_year_ptr|| !group_ptr->prev_song_ptr)
    {
        print_file<<"\t#"<<count<<": "<<search->m_song->value;
    }
    else
    {
        bool check = false;
        while(search && search->prev_song_ptr && search->prev_by_year_ptr)
        {
            print_file<<"\t#"<<count<<": "<<search->m_song->value;
            check = true;
            count++;
            search = search->prev_by_year_ptr;
        }
        if(check)
            print_file<<"\t#"<<count<<": "<<search->m_song->value;

    }
    print_file<<"Rewind list contained " << count << " song(s)."<<std::endl;
}

/*
ARGUMENTS:
-group_ptr points to the location in a particular song group to start printing from
-print_file points to an ostream to print output to
BEHAVIOR:
Print a rewind list by following the next_by_artist_ptr pointers, starting from the song
pointed to by group_ptr.
RETURN:
None
*/
void PrintGroupMarathon(SongGroupNode* group_ptr,std::ostream& print_file){
    if(!group_ptr)
        return;
    int c = 1;
    int count = 1;
    bool check = false;
    print_file<<"Marathon list for "<< group_ptr->m_song->value.getArtist()<<std::endl;
    SongGroupNode* search = group_ptr;
    if(!group_ptr->next_by_artist_ptr)
        print_file<<"\t#"<<c<<": "<<search->m_song->value;
    while(search && search->next_by_artist_ptr)
    {
        print_file<<"\t#"<<c<<": "<<search->m_song->value;
        c++;
        check = true;
        count++;
        search = search->next_by_artist_ptr;
    }
    if(check)
        print_file<<"\t#"<<c<<": "<<search->m_song->value;
    print_file<<"Marathon list contained "<<count<< " song(s)."<<std::endl;
}

/*
ARGUMENTS:
-group_head points to the first SongGroupNode* of the group to analyze
-print_file points to an ostream to print output to
BEHAVIOR:
Find the rewind list with the most songs in the song group pointed to by group_head.
Print the rewind list and the song count to print_file.
RETURN:
None
*/
void PrintLongestGroupRewind(GroupListNode* group_head,std::ostream& print_file){
    SongGroupNode* search = group_head->value;
    SongGroupNode* streak = group_head->value;
    int count = 1;
    int max = 0;
    while(search && search->next_song_ptr)
        search = search->next_song_ptr;
    SongGroupNode* tail = search;
    while(tail && tail->prev_song_ptr)
    {
        count = 1;
        search = tail;
        while(search && search-> prev_song_ptr && search->prev_by_year_ptr)
        {
            count++;
            search = search->prev_by_year_ptr;
        }
        if(max < count)
        {
            max = count;
            streak = tail;
        }
        tail = tail->prev_song_ptr;
    }
    int c = 1;
    print_file<<"Maximum size rewind list is for year " <<streak->m_song->value.getYear()<<std::endl;
    while(streak && streak->prev_by_year_ptr)
    {
        print_file<<"\t#"<<c<<": "<<streak->m_song->value;
        c++;
        streak = streak->prev_by_year_ptr;
    }
    print_file<<"\t#"<<c<<": "<<streak->m_song->value;
    print_file<<"Longest rewind list contained " << max << " song(s)."<<std::endl;
}

///////////////////////////////CLEANUP/////////////////////////////////////////

/*
ARGUMENTS:
-song_groups points to a singly-linked list of song group heads
BEHAVIOR:
Delete every group. Do not leak. Modifies all groups and the song_groups list.
Update every Song to reflect not being in a group.
RETURN:
None
*/
void DeleteAllSongGroups(GroupListNode*& song_groups){
    while(song_groups)
    {
        GroupListNode* search = song_groups;
        SongGroupNode* temp_sgn = search->value;
        song_groups = song_groups->ptr;
        while(temp_sgn)
        {
            if(temp_sgn->m_song){
                temp_sgn->m_song->value.setUsedInGroup(false);
                temp_sgn->m_song->value.updateSongGroupPointer(NULL);
        }
        SongGroupNode* temp = temp_sgn->next_song_ptr;
        delete temp_sgn;
        temp_sgn = temp;
        }
        delete search;
    }
    if(song_groups)
        delete song_groups;
}