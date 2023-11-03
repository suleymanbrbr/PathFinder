# PathFinder
The aim of this project is to use linked lists and doubly linked lists. You will employ these structures to implement an airline passenger’s pathfinder program. The number of flights cannot be known; therefore, you will use dynamic memory allocation, i.e., new/delete operations, to create and delete these structures. Don’t forget, we don’t want memory leaks in the software, so each memory region allocated by a new operation should be freed by a delete operation once it is not necessary anymore.

Our passenger is a time traveler, but she is still low on budget. Therefore she wants to optimize her travel costs by searching for the cheapest route while leveraging her ability. You are provided with blocks of code, but you need to implement data structure and search functionalities. There are two structs; airline and flights. You should use those structs as they are for nodes of your linked list structure. Provided file reader returns two vectors; vector<string> airlines and vector<vector<flight>> flights. You need to construct a linked-list structure from those two vectors. Data is stored in these vectors as the following;

<img width="376" alt="Screenshot 2023-11-03 at 14 07 13" src="https://github.com/suleymanbrbr/PathFinder/assets/111366311/953a12f7-2bd3-4639-93f9-7cb3ca202695">

You will use a 2D airline and flights list. The airlines will be stored in a single linked list and their flights will be stored in a double linked list. Flights must be kept ascending in time in the list. First flight in the list must be the earliest and last flight must be the latest. Here is a visual description of the data structure you need to implement.

<img width="436" alt="Screenshot 2023-11-03 at 14 07 40" src="https://github.com/suleymanbrbr/PathFinder/assets/111366311/2fbc4328-4286-4766-8265-56677cb15303">

Your data structure need to have these functionalities;
• Build the data structure from vectors and keep it always sorted by time.
• Add flights manually. If there is no given airline, create it and append it to the end of the list of airlines. 
• Remove flights. If there is no flight left for an airline, remove the airline from the list of airlines.
• Print list. Print the whole data structure in the format that will be given in executions.
• Delete list. Delete whole list and free every pointer allocated with new.

You need to implement a pathfinder function that finds the cheapest route with at most requested transfers using the linked list you implemented. You may start from any flight node from any airline in the list that starts from the requested location. Since our passenger is a time traveler, when you take a flight and use it on your route, you can continue with any flight regardless of its time that starts from the same location. However, your route has a maximum number of transfers. You can’t report a route that has more transfers than requested. You can find exact examples in executions files.

<img width="471" alt="Screenshot 2023-11-03 at 14 09 23" src="https://github.com/suleymanbrbr/PathFinder/assets/111366311/87d78619-a4d0-42fd-8a08-af12ec4a8e4e">

<img width="442" alt="Screenshot 2023-11-03 at 14 09 46" src="https://github.com/suleymanbrbr/PathFinder/assets/111366311/c9bcf32d-75ef-45ce-95b6-2d1e13711917">
