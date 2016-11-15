class Date
{
	public:

	Date();
	Date(size_t year,size_t month,size_t day);

	size_t year;
	size_t month;
	size_t day;
};

class User{
	public:
		size_t getUserId();
	private:
		size_t id;
		string name;
		std::vector<Book> borrowed_books; 
};


class Book{
public:
	Book();
	Book(string name);
	~Book()

	size_t getBookId() const;
	User*  getBookOwner() const;

	void setBorrowingDate(Date& date);
	Date& getBorrowingDate() const;

private:
	size_t id;
	string name;
	User* owner;
	Date  borrowing_date;


};

const MaxUserBooks = 3;
const MaxDaysToExpire = 30;

class LibraryModel{
public:
	std::vector<User>& getUsers() const;
	std::vector<Book>& getBooks() const;

	void addUser(User& user);
	void delUser(User& user);

	void addBook(Book& book);
	void delBook(Book& book);

	void LentABookToUser(Book& book,User& user);
	void TakeABookFromUser(Book& book,User& user);

	std::vector<User> getUsersWithExpiredBooks() const;

private:
	std::vector<User> library_users;
	std::vector<Book> library_books;
	Date current_date;
	
};


class LibraryView{
public:
	void ShowAllUsers() const;
	void ShowAllBooks() const;
	void ShowUsersWithExpiredBooks() const;
private:
	LibraryController* controller;
};


class LibraryController{
public:
	void LentABookToUser(Book& book,User& user) { return model->LentABookToUser(book,user); }
	void TakeABookFromUser(Book& book,User& user) { return model->TakeABookFromUser(book,user); }

	std::vector<User>& getUsers() const { return model->getUsers(); }
	std::vector<Book>& getBooks() const { return model->getBooks(); }

	std::vector<User>& getUsersWithExpiredBooks() const { return model->getUsersWithExpiredBooks();}

private:
	LibraryModel* model;
};

void LibraryView::ShowAllUsers() const {
	std::vector<User>& users = controller->getUsers();
	if(!users.empty()){
		std::cout << "Users:" << std::endl;
		for(auto iter = users.begin(); iter != users.end(); iter++)
		{
			std::cout << iter->name << " with id: " << iter->id << std::endl;
		}
	}
	else {
		std::cout << "There is no users in this library yet." << std::endl;

	}
}

void LibraryView::ShowAllBooks() const {
	std::vector<Book>& books = controller->getBooks();
	if(!books.empty()){
		std::cout << "Books:" << std::endl;
		for(auto iter = books.begin(); iter != books.end(); iter++)
		{
			std::cout << iter->name << " with id: " << iter->id << std::endl;
		}
	}
	else {
		std::cout << "There is no books in this library yet." << std::endl;

	}
}

void LibraryView::ShowUsersWithExpiredBooks() const{
	std::vector<User>& users = controller->getUsersWithExpiredBooks();
	if(!users.empty()){
		std::cout << "Users who expired his/her book(s): " << std::endl;
		for(auto iter = users.begin();iter != users.end(); iter++)
		{
			std::cout << iter->name << " with id: " << iter->id << std::endl;
		}
	}
	else{
		std::cout << "There is no users who expired his/her book(s) "<< std::endl;
	}
}

std::vector<User>& LibraryModel::getUsers() const { return library_users; }
std::vector<Book>& LibraryModel::getBooks() const { return library_books; }

void LibraryModel::addUser(User& user)
{
	for(auto iter=library_users.begin();iter != library_users.end(); iter++)
	{
		if(iter->id == user.id)
		{
			if(iter->name == user.name){
				//std::cout << "User already exists!" << std::endl; //TODO Send Message to View
				return;
			}
			else{
				//std::cout << "User already exists with name: " << iter->name << std::endl; //TODO Send Message to View
				return;
			}			
		}
	}
	library_users.push_back(user);
}

void LibraryModel::delUser(User& user)
{
	for(auto iter=library_users.begin();iter != library_users.end();iter++)
	{
		if(iter->id == user.id)
		{
			library_users.erase(*iter);
			return;
		}
	}

	//std::cout << "User does not exists!" << std::endl; //TODO Send Message to View
}

void LibraryModel::addBook(Book& book)
{
	for(auto iter=library_books.begin();iter != library_books.end(); iter++)
	{
		if(iter->id == book.id)
		{
			if(iter->name == book.name){
				//std::cout << "Book already exists!" << std::endl; //TODO Send Message to View
				return;
			}
			else{
				//std::cout << "Book already exists with name: " << iter->name << std::endl; //TODO Send Message to View
				return;
			}			
		}
	}
	library_books.push_back(book);
}

void LibraryModel::delBook(Book& book)
{
	for(auto iter=library_books.begin();iter != library_books.end();iter++)
	{
		if(iter->id == book.id)
		{
			library_books.erase(*iter);
			return;
		}
	}

	//std::cout << "Book does not exists!" << std::endl; //TODO Send Message to View
}

void LibraryModel::LentABookToUser(Book& book,User& user)
{
	if(book.owner != (User*)NULL){
		//std::cout << "This book is aleady owed by someone!" << std::endl; //TODO Send Message to View
		return;
	}

	if(user.borrowed_books.size() > MaxUserBooks)
	{
		//std::cout << "User owes too much books!" << std::endl; //TODO Send Message to View
		return;
	}

	for(auto iter = user.borrowed_books.begin();iter != user.borrowed_books.end();iter++)
	{
		if(iter->id == book.id)
		{
			//std::cout << "User user already owes this book" << std::endl; //TODO Send Message to View
			return;
		}
	}

	user.borrowed_books.push_back(book);
	book.owner = &user;
	
}


void TakeABookFromUser(Book& book,User& user)
{
	if(book.owner->id != user.id)
	{
		//std::cout << "This book owes to another user" << std::endl; //#TODO Send Message to View
		return;
	}

	for(auto iter = user.borrowed_books.begin();iter != user.borrowed_books.end();iter++)
	{
		if(iter->id == book.id){
			user.borrowed_books.erase(*iter);
			break;
		}
	}
	book.owner = (User*) NULL;
}

std::vector<User> LibraryModel::getUsersWithExpiredBooks() const
{
	size_t current_days = 0;
	size_t lasted_days = 0;

	current_days = current_date.year*365 + current_date.month*30 + current_date.day;

	std::vector<User> expired_users;
	for(auto iter = library_users.begin();iter != library_users.end();iter++)
	{
		for(size_t i = 0;i < iter->borrowed_books.size();i++)
		{
			lasted_days = iter->borrowed_books[i].year*356;
			lasted_days += iter->borrowed_books[i].month*30;
			lasted_days += iter->borrowed_books[i].day;

			if(lasted_days > MaxDaysToExpire){
				expired_users.push_back(*iter);
				break;
			}
		}
	}
	return expired_users;
}



