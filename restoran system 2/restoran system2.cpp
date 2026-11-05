#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
using namespace std;



class MyException {
	string text;
	int line;
	string datetime;
public:
	MyException() {
		this->text = "Unknown exception";
		this->line = 0;
		this->datetime = __DATE__;
	}
	MyException(string text, int line = 0) {
		this->text = text;
		this->line = line;
		this->datetime = __DATE__;

	}

	void what()const {
		cout << "Error Message: " << text << endl;
		cout << "Line: " << line << endl;
		cout << "Date: " << datetime << endl;
	}
};



class Ingredient {
	string id;   // kodu
	string name;
	double pricePerKg;  // kg gore qiymet ,anbara yeni inqrediyent alanda restoran büdcəsindən bu qiymətə uyğun pul çıxılacaq).

public:

	Ingredient() {
		id = "";
		name = "";
		pricePerKg = 0;
	}

	Ingredient(string id, string name, double pricePerKg) {
		setId(id);
		setName(name);
		setPriceKg(pricePerKg);
	}

	void setId(const string& id) {
		if (id.empty()) {
			throw MyException("Xeta: Inqrediyent ID-si bos ola bilmez!");
		}
		this->id = id;
	}


	string getId() const {
		return id;
	}

	void setName(const string& name) {
		if (name.empty()) {
			throw MyException("Xeta: Inqrediyent adi bos ola bilmez!");
		}
		this->name = name;
	}

	string getName() const {
		return name;
	}

	void setPriceKg(double pricePerKg) {
		if (pricePerKg > 0) {
			this->pricePerKg = pricePerKg;
		}
		else {
			throw(MyException("Xeta: Qiymet 0 - dan kicik ola  bilmez!"));
		}
	}

	double getPriceKg()const {
		return pricePerKg;
	}

	void getInfo()const {
		cout << "[İnqrediyent Kodu: " << id << "] -- " << name << " (" << pricePerKg << " AZN/kq)" << endl;
	}

	double calculateCost(double weightInKg) const {
		if (weightInKg <= 0) {
			throw MyException("Xeta: Ceki 0-dan boyuk olmalidir!");
		}
		return pricePerKg * weightInKg;
	}

};

struct RecipeItem {  // resept
	string ingredientId;    // Anbardaki ingredient-in ID-si
	double requiredAmountKg; // Yemek ucun lazim olan miqdar (kg)

	RecipeItem() {
		ingredientId = "";
		requiredAmountKg = 0;
	}

	RecipeItem(string ingredientId, double requiredAmountKg) {
		if (ingredientId.empty()) {
			throw MyException("Xeta: Recipe ingredient ID-si bos ola bilmez!");
		}

		if (requiredAmountKg <= 0) {
			throw MyException("Xeta: Ingredient miqdari 0-dan boyuk olmalidir!");
		}

		this->ingredientId = ingredientId;
		this->requiredAmountKg = requiredAmountKg;
	}



};


class Food {
	string id;
	string name;
	string description; // tesvir
	double price;
	string category; // aid oldugu yer sirniyyat fln
	vector<RecipeItem> recipe; // Yeməyin hazir olmasi ucun lazm inqrediyentləri
	bool available; // movcud olub olmadigini yoxluyur

public:
	Food() {
		id = "";
		name = "";
		description = "";
		price = 0;
		category = "";
		available = true;
	}

	Food(string id, string name, string description, double price, string category) {
		setId(id);
		setName(name);
		setDescription(description);
		setPrice(price);
		setCategory(category);
		available = true;

	}


	void setId(const string& id) {
		if (id.empty()) {
			throw MyException("Xeta: Yemek ID-si bos ola bilmez!");
		}
		this->id = id;
	}


	string getId() const {
		return id;
	}

	void setName(const string& name) {
		if (name.empty()) {
			throw MyException("Xeta: Yemek adi daxil edilmelidir!");
		}
		this->name = name;
	}

	string getName() const {
		return name;
	}

	void setDescription(const string& description) {
		this->description = description;
	}

	string getDescription() const {
		return description;
	}

	void setCategory(const string& category) {
		if (category.empty()) {
			throw MyException("Xeta: Kateqoriya qeyd olunmalidir!");
		}
		this->category = category;
	}

	string getCategory() const {
		return category;
	}

	void setPrice(double price) {
		if (price > 0) {
			this->price = price;
		}
		else {
			throw(MyException("Xeta: Yemeyin qiymeti 0-dan boyuk olmalidir!"));
		}
	}

	double getPrice() const {
		return price;
	}

	const vector<RecipeItem>& getRecipe() const {
		return recipe;
	}

	void setAvailable(bool status) {
		available = status;
	}

	bool isAvailable() const {
		return available;
	}


	//  Reseptin bos olub-olmadigini yoxlayir
	bool hasRecipe() const {
		return recipe.size() != 0;
	}

	// Resepte ingredient elave edir
	void addRecipeItem(const RecipeItem& item) {

		if (item.ingredientId.empty()) {
			throw MyException("Xeta: Ingredient ID-si bos ola bilmez!");
		}

		if (item.requiredAmountKg <= 0) {
			throw MyException("Xeta: Ingredient miqdari 0-dan boyuk olmalidir!");
		}

		// Eyni ingredient-in resepte tekrar elave olunmasinin qarsisini alir
		for (const RecipeItem& r : recipe) {
			if (r.ingredientId == item.ingredientId) {
				throw MyException("Xeta: Bu ingredient artiq reseptde movcuddur!");
			}
		}

		recipe.push_back(item);
	}

	// Reseptden ingredient silir
	void removeRecipeItem(const string& ingredientId) {

		for (int i = 0; i < recipe.size(); i++) {
			if (recipe[i].ingredientId == ingredientId) {
				recipe.erase(recipe.begin() + i);
				return;
			}
		}

		throw MyException("Xeta: Reseptde bu ingredient tapilmadi!");
	}

	// Reseptde ingredient miqdarini deyisir
	void updateRecipeItem(const string& ingredientId, double newAmountKg) {

		if (newAmountKg <= 0) {
			throw MyException("Xeta: Ingredient miqdari 0-dan boyuk olmalidir!");
		}

		for (RecipeItem& r : recipe) {
			if (r.ingredientId == ingredientId) {
				r.requiredAmountKg = newAmountKg;
				return;
			}
		}

		throw MyException("Xeta: Reseptde bu ingredient tapilmadi!");
	}


	void showInfo() const {

		cout << "----------------------------------------\n"
			<< " Yemek Kodu  : " << id << "\n"
			<< " Adi         : " << name << "\n"
			<< " Kateqoriya  : " << category << "\n"
			<< " Qiymet      : " << price << " AZN\n"
			<< " Tesvir      : " << description << "\n"
			<< " Status      : " << (available ? "Movcuddur" : "Movcud deyil") << "\n"
			<< "----------------------------------------\n"
			<< " Resept İnqrediyentleri:\n";

		for (const RecipeItem& r : recipe) {
			cout << "Ingredient ID: " << r.ingredientId
				<< " | Miqdar: " << r.requiredAmountKg << " kg" << endl;
		}
	}

	double calculatePrice(int portionCount) const {
		if (portionCount <= 0) {
			throw(MyException("Xeta: Porsiya sayi 0-dan boyuk olmalidir!"));
		}

		return price * portionCount;
	}

	void WriteToFile(const vector<Food>& foods, string path = "foods.txt") {

		ofstream fs(path, ios::out);

		if (fs.is_open()) {

			for (const Food& f : foods) {

				fs << f.getId() << "#";
				fs << f.getName() << "#";
				fs << f.getDescription() << "#";
				fs << f.getPrice() << "#";
				fs << f.getCategory() << "#";
				fs << f.isAvailable() << "#";
				fs << f.getRecipe().size();

				for (const RecipeItem& r : f.getRecipe()) {
					fs << "#" << r.ingredientId;
					fs << "#" << r.requiredAmountKg;
				}

				fs << endl;
			}

			fs.close();
		}
		else {
			throw "File couldn't open...";
		}
	}

	void ReadFromFile(vector<Food>& foods, string path = "foods.txt") {

		ifstream fs(path);

		if (fs.is_open()) {

			while (!fs.eof()) {

				string row;
				getline(fs, row);

				vector<string> values;
				string value = "";

				for (char character : row) {

					if (character == '#') {
						values.push_back(value);
						value = "";
					}
					else {
						value += character;
					}
				}

				values.push_back(value);

				if (values.size() >= 7 && values[0] != "") {

					Food f(
						values[0],
						values[1],
						values[2],
						stod(values[3]),
						values[4]
					);

					f.setAvailable(stoi(values[5]));

					int recipeCount = stoi(values[6]);

					int counter = 7;

					for (int i = 0; i < recipeCount; i++) {

						RecipeItem item;

						item.ingredientId = values[counter];
						item.requiredAmountKg = stod(values[counter + 1]);

						f.addRecipeItem(item);

						counter += 2;
					}

					foods.push_back(f);
				}
			}

			fs.close();
		}
		else {
			throw "File not found...";
		}
	}








};


// sklad

class Inventory {
	vector<Ingredient> ingredients; //Anbarda olan inqrediyentlərin siyahısı.
	vector<double> quantities; // anbarda ne qeder qalib onu gosterir

	// Ingredient-in siyahida hansi indeksde oldugunu tapir
	int findIngredientIndex(const string& id) const {
		for (size_t i = 0; i < ingredients.size(); i++) {
			if (ingredients[i].getId() == id) {
				return (int)i;
			}
		}
		return -1;
	}

public:
	void addIngredient(const Ingredient& newIngredient, double initialQuantity) {

		if (newIngredient.getId().empty()) {
			throw MyException("Xeta: Ingredient ID-si bos ola bilmez!");
		}

		if (initialQuantity < 0) {
			throw MyException("Xeta: Ilkin miqdar menfi ola bilmez!");
		}

		if (findIngredientIndex(newIngredient.getId()) != -1) {
			throw MyException("Xeta: Bu ID-li ingredient anbarda artiq movcuddur!");
		}

		ingredients.push_back(newIngredient);
		quantities.push_back(initialQuantity);
		cout << "Inqrediyent anbara ugurla elave edildi!" << endl;
	}

	// miqdar artirir
	void increaseStock(const string& id, double amount) {
		if (amount <= 0)
		{
			throw MyException("Xeta: Artirilacaq miqdar 0-dan boyuk olmalidir!");
		}
		// Ingredient-i ayrica axtaririq
		int index = findIngredientIndex(id);

		if (index == -1) {
			throw MyException("Xeta: Bu ID-li ingredient anbarda tapilmadi!");
		}

		quantities[index] += amount;

		cout << "Stok ugurla artirildi! Cari miqdar: "
			<< quantities[index] << " kg" << endl;
	}


	// azaldir

	void decreaseStock(const string& id, double amount) {
		if (amount <= 0)
		{
			throw MyException("Xeta: Azaldilacaq miqdar 0-dan boyuk olmalidir!");
		}
		int index = findIngredientIndex(id);

		if (index == -1) {
			throw MyException("Xeta: Bu ID-li ingredient anbarda tapilmadi!");
		}

		if (quantities[index] < amount) {
			throw MyException("Xeta: Anbarda kifayet qeder ingredient yoxdur!");
		}

		quantities[index] -= amount;

		cout << "Stok ugurla azaldildi! Qalan miqdar: "
			<< quantities[index] << " kg" << endl;
	}


	// ID-ye gore ingredient obyektini qaytarir
	const Ingredient& getIngredient(const string& id) const {

		int index = findIngredientIndex(id);

		if (index == -1) {
			throw MyException("Xeta: Ingredient anbarda tapilmadi!");
		}

		return ingredients[index];
	}

	bool containsIngredient(const string& id) const {
		return findIngredientIndex(id) >= 0;
	}


	//ID-ye gore stok miqdarini qaytarir
	double getQuantity(const string& id) const {

		int index = findIngredientIndex(id);

		if (index == -1) {
			throw MyException("Xeta: Ingredient anbarda tapilmadi!");
		}

		return quantities[index];
	}


	// Lazim olan miqdar anbarda varmi?
	bool hasEnoughStock(const string& id, double requiredAmount) const {

		if (requiredAmount <= 0) {
			throw MyException("Xeta: Teleb olunan miqdar 0-dan boyuk olmalidir!");
		}

		int index = findIngredientIndex(id);

		if (index == -1) {
			return false;
		}

		return quantities[index] >= requiredAmount;
	}


	//Yemeyin reseptine gore butun ingredientleri yoxlayir
	bool canPrepareFood(const Food& food) const {

		if (!food.hasRecipe()) {
			return false;
		}

		for (const RecipeItem& item : food.getRecipe()) {

			if (!hasEnoughStock(item.ingredientId, item.requiredAmountKg)) {
				return false;
			}
		}

		return true;
	}


	//  Yemek hazirlananda reseptdeki ingredientleri stokdan cixir
	void consumeRecipe(const Food& food) {

		// Evvelce butun ingredientleri yoxlayiriq
		if (!canPrepareFood(food)) {
			throw MyException("Xeta: Yemek ucun kifayet qeder ingredient yoxdur!");
		}

		// Hamisi kifayet edirse, yalniz bundan sonra stokdan cixir
		for (const RecipeItem& item : food.getRecipe()) {

			int index = findIngredientIndex(item.ingredientId);

			quantities[index] -= item.requiredAmountKg;
		}

		cout << food.getName() << " ucun ingredientler stokdan cixarildi!" << endl;
	}

	void checkStock() const {
		cout << "\n======= ANBAR =======" << endl;
		if (ingredients.empty()) {
			cout << "Anbar bosdur." << endl;
			return;
		}
		for (size_t i = 0; i < ingredients.size(); ++i) {
			cout << "ID: " << ingredients[i].getId()
				<< " | Ad: " << ingredients[i].getName()
				<< " | Miqdar: " << quantities[i] << " kg" << endl;
		}
		cout << "=============================\n" << endl;
	}

	void showInventory() {
		cout << "\n===== ANBAR =====" << endl;

		for (int i = 0; i < ingredients.size(); i++) {
			ingredients[i].getInfo();

			cout << "Miqdar: " << quantities[i] << " kg" << endl;
			cout << "----------------------" << endl;
		}
	}


	void WriteToFile(string path = "inventory.txt") {

		ofstream fs(path, ios::out);

		if (fs.is_open()) {

			for (size_t i = 0; i < ingredients.size(); i++) {

				fs << ingredients[i].getId() << "#"
					<< ingredients[i].getName() << "#"
					<< ingredients[i].getPriceKg() << "#"
					<< quantities[i] << "\n";
			}
		}
		else {
			throw string("File couldn't open...");
		}

		fs.close();
	}


	void ReadFromFile(string path = "inventory.txt") {

		ifstream fs(path);

		if (fs.is_open()) {

			ingredients.clear();
			quantities.clear();

			while (fs.eof() == false) {

				string row;
				getline(fs, row);

				int counter = 0;

				string id;
				string name;
				string str_price;
				string str_quantity;

				for (char character : row) {

					if (character != '#' && counter == 0) {
						id += character;
					}
					else if (character != '#' && counter == 1) {
						name += character;
					}
					else if (character != '#' && counter == 2) {
						str_price += character;
					}
					else if (character != '#' && counter == 3) {
						str_quantity += character;
					}
					else {
						counter++;
					}
				}

				if (str_price.size() != 0 && str_quantity.size() != 0) {

					double price = stod(str_price);
					double quantity = stod(str_quantity);

					Ingredient ingredient(id, name, price);

					addIngredient(ingredient, quantity);
				}
			}
		}
		else {
			throw string("File not found...");
		}

		fs.close();
	}

};


class Menu {
	vector<Food> foods;  // umumi siyahi
public:

	void addFood(const Food& newFood) {

		if (newFood.getId().empty()) {
			throw MyException("Xeta: Yemek ID-si bos ola bilmez!");
		}

		for (const Food& f : foods) {
			if (f.getId() == newFood.getId()) {
				throw MyException("Xeta: Bu ID-li yemek menyuda artiq movcuddur!");
			}
		}

		foods.push_back(newFood);
		cout << "Yemek menyuya ugurla elave olundu!" << endl;
	}



	void removeFood(const string& id) {

		if (id.empty()) {
			throw MyException("Xeta: Silinecek yemek ID-si bos ola bilmez!");
		}

		for (size_t i = 0; i < foods.size(); ++i) {

			if (foods[i].getId() == id) {
				foods.erase(foods.begin() + i);
				cout << "Yemek menyudan ugurla silindi!" << endl;
				return;
			}
		}

		throw MyException("Xeta: Silinecek yemek tapilmadi!");
	}


	Food& findFoodById(const string& id) {
		for (size_t i = 0; i < foods.size(); ++i) {
			if (foods[i].getId() == id) {
				return foods[i];
			}
		}
		throw MyException("Xeta: Axtardiginiz yemek tapilmadi!");
	}

	const Food& findFoodById(const string& id) const {

		for (size_t i = 0; i < foods.size(); ++i) {

			if (foods[i].getId() == id) {
				return foods[i];
			}
		}

		throw MyException("Xeta: Axtardiginiz yemek tapilmadi!");
	}


	//  Yemeyin melumatlarini yenileyir
	void updateFood(const string& id, string newName,
		string newDescription, double newPrice, string newCategory) {

		Food& food = findFoodById(id);

		food.setName(newName);
		food.setDescription(newDescription);
		food.setPrice(newPrice);
		food.setCategory(newCategory);

		cout << "Yemek melumatlari ugurla yenilendi!" << endl;
	}


	// Anbarin veziyyetine gore yemeklerin statusunu yenileyir
	void updateAvailability(const Inventory& inventory) {

		for (Food& food : foods) {

			food.setAvailable(inventory.canPrepareFood(food));
		}
	}


	// yalniz movcud yemekleri gosterir
	void showAvailableFoods() const {

		cout << "\n======= MOVCUD YEMEKLER =======" << endl;

		bool found = false;

		for (const Food& food : foods) {

			if (food.isAvailable()) {
				food.showInfo();
				found = true;
			}
		}

		if (!found) {
			cout << "Hazirda movcud yemek yoxdur!" << endl;
		}

		cout << "===============================" << endl;
	}




	void showMenu() const {
		cout << "\n================ RESTORAN MENYUSU ================" << endl;
		if (foods.empty()) {
			cout << "Menyu hazirda bosdur." << endl;
			cout << "==================================================\n" << endl;
			return;
		}

		for (size_t i = 0; i < foods.size(); ++i) {
			foods[i].showInfo();
		}
		cout << "==================================================\n" << endl;
	}

	const vector<Food>& getFoods() const {
		return foods;
	}

	void WriteMenuToFile(const Menu& menu, string path = "menu.txt") {

		ofstream fs(path, ios::out);

		if (fs.is_open()) {

			for (const Food& f : menu.getFoods()) {

				fs << f.getId() << "#";
				fs << f.getName() << "#";
				fs << f.getDescription() << "#";
				fs << f.getPrice() << "#";
				fs << f.getCategory() << "#";
				fs << f.isAvailable() << "#";
				fs << f.getRecipe().size();

				for (const RecipeItem& r : f.getRecipe()) {
					fs << "#" << r.ingredientId;
					fs << "#" << r.requiredAmountKg;
				}

				fs << endl;
			}

			fs.close();
		}
		else {
			throw "File couldn't open...";
		}
	}


	void ReadMenuFromFile(Menu& menu, string path = "menu.txt") {

		ifstream fs(path);

		if (fs.is_open()) {

			while (!fs.eof()) {

				string row;
				getline(fs, row);

				vector<string> values;
				string value = "";

				for (char character : row) {

					if (character == '#') {
						values.push_back(value);
						value = "";
					}
					else {
						value += character;
					}
				}

				values.push_back(value);

				if (values.size() >= 7 && values[0] != "") {

					Food f(
						values[0],
						values[1],
						values[2],
						stod(values[3]),
						values[4]
					);

					f.setAvailable(stoi(values[5]));

					int recipeCount = stoi(values[6]);
					int counter = 7;

					for (int i = 0; i < recipeCount; i++) {

						RecipeItem item;

						item.ingredientId = values[counter];
						item.requiredAmountKg = stod(values[counter + 1]);

						f.addRecipeItem(item);

						counter += 2;
					}

					menu.addFood(f);
				}
			}

			fs.close();
		}
		else {
			throw "File not found...";
		}
	}


};

// sebetdekimehsul
class CartItem {
	Food food;          // Səbətə əlavə edilən yemək obyekti
	int quantity;       // Porsiya sayı
	string customNote;  // Xususi istek ( "extra pendirli", "soğansız")
	vector<RecipeItem> extraIngredients; // Elave ingredientler
public:
	CartItem() {
		quantity = 0;
		customNote = "";
	}

	CartItem(const Food& food, int quantity, const string& customNote = "") {
		this->food = food;
		setQuantity(quantity);
		this->customNote = customNote;
	}

	void setFood(const Food& food) {
		this->food = food;
	}

	const Food& getFood() const {
		return food;
	}

	void setQuantity(int quantity) {
		if (quantity <= 0) {
			throw MyException("Xeta: Porsiya sayi 0-dan boyuk olmalidir!");
		}
		this->quantity = quantity;
	}

	int getQuantity() const {
		return quantity;
	}

	void setCustomNote(const string& customNote) {
		this->customNote = customNote;
	}

	string getCustomNote() const {
		return customNote;
	}

	// Yemeye elave ingredient elave edir
	void addExtraIngredient(const string& ingredientId, double amountKg) {

		if (ingredientId.empty()) {
			throw MyException("Xeta: Ingredient ID-si bos ola bilmez!");
		}

		if (amountKg <= 0) {
			throw MyException("Xeta: Elave ingredient miqdari 0-dan boyuk olmalidir!");
		}

		// Eyni ingredient artiq elave olunubsa, miqdarini artirir
		for (RecipeItem& item : extraIngredients) {

			if (item.ingredientId == ingredientId) {
				item.requiredAmountKg += amountKg;
				return;
			}
		}

		extraIngredients.push_back(RecipeItem(ingredientId, amountKg));
	}

	// elave ingredientlerin siyahisini qaytarir
	const vector<RecipeItem>& getExtraIngredients() const {
		return extraIngredients;
	}

	// Elave ingredientlerin miqdarini deyisir
	void updateExtraIngredient(const string& ingredientId, double newAmountKg) {

		if (newAmountKg <= 0) {
			throw MyException("Xeta: Ingredient miqdari 0-dan boyuk olmalidir!");
		}

		for (RecipeItem& item : extraIngredients) {

			if (item.ingredientId == ingredientId) {
				item.requiredAmountKg = newAmountKg;
				return;
			}
		}

		throw MyException("Xeta: Elave ingredient tapilmadi!");
	}

	// Elave ingredienti silir
	void removeExtraIngredient(const string& ingredientId) {

		for (size_t i = 0; i < extraIngredients.size(); i++) {

			if (extraIngredients[i].ingredientId == ingredientId) {
				extraIngredients.erase(extraIngredients.begin() + i);
				return;
			}
		}

		throw MyException("Xeta: Silinecek elave ingredient tapilmadi!");
	}


	// umumi qiymet hesablama
	double getTotalPrice() const {
		return food.getPrice() * quantity;
	}

	void showInfo() const {
		cout << "Yemek: " << food.getName()
			<< " | Qiymet: " << food.getPrice() << " AZN"
			<< " | Porsiya: " << quantity;

		// eger xüsusi qeyd varsa, onu da ekrana cixarirq
		if (customNote.size() > 0) {
			cout << " | Qeyd: " << customNote;
		}

		cout << " | Cemi: " << getTotalPrice() << " AZN" << endl;
	}




};




class Cart {
private:
	vector<CartItem> items;

public:
	// eyni yemek ve eyni qeyd varsa miqdari artir
	void addItem(Food food, int quantity, string customNote = "") {
		if (food.isAvailable() == false) {
			throw MyException("Bu yemek hazirda movcud deyil.");
		}

		if (quantity <= 0) {
			throw MyException("Xeta: Miqdar 0-dan boyuk olmalidir");
		}

		for (CartItem& item : items) {
			if (item.getFood().getId() == food.getId() &&
				item.getCustomNote() == customNote) {

				item.setQuantity(item.getQuantity() + quantity);
				return;
			}
		}

		items.push_back(CartItem(food, quantity, customNote));
	}

	void removeItem(string foodId) {
		for (int i = 0; i < items.size(); i++) {
			if (items[i].getFood().getId() == foodId) {
				items.erase(items.begin() + i);
				return;
			}
		}

		throw MyException("Yemek sebetde tapilmadi.");
	}

	//  mehsulun miqdarini deyismek
	void updateQuantity(string foodId, int newQuantity) {
		if (newQuantity <= 0) {
			throw MyException("Xeta: Miqdar 0-dan boyuk olmalidir");
		}

		for (CartItem& item : items) {
			if (item.getFood().getId() == foodId) {
				item.setQuantity(newQuantity);
				return;
			}
		}

		throw MyException("Yemek sebetde tapilmadi.");
	}

	// sebete elave ingredient elave etmek
	void addExtraIngredient(string foodId, RecipeItem extra) {
		for (CartItem& item : items) {
			if (item.getFood().getId() == foodId) {
				item.addExtraIngredient(extra.ingredientId, extra.requiredAmountKg);
				return;
			}
		}

		throw MyException("Yemek sebetde tapilmadi.");
	}

	double getTotalPrice() const {
		double total = 0;

		for (const CartItem& item : items) {
			total += item.getTotalPrice();
		}

		return total;
	}

	void showCart() const {
		if (items.empty()) {
			cout << "Sebet bosdur!" << endl;
			return;
		}

		cout << "========================================\n"
			<< "              SEBET MELUMATI            \n"
			<< "========================================" << endl;

		for (const CartItem& item : items) {
			item.showInfo();
			cout << "------------------" << endl;
		}

		cout << "Umumi mebleg: " << getTotalPrice() << " AZN" << endl;
		cout << "========================================" << endl;
	}

	void clearCart() {
		items.clear();
	}

	// sebetin bos olub-olmadigini yoxlayir
	bool isEmpty() const {
		return items.empty();
	}

	vector<CartItem> getItems() const {
		return items;
	}
};

// Sifariş
class Order {
	int orderId;
	vector<CartItem> items;
	double totalAmount; // odenilecek mebleg
	string orderDate; // sifarisinverildyi tarix ve saat
public:
	Order() {
		orderId = 0;
		totalAmount = 0.0;
		orderDate = "";
	}

	Order(int orderId, const vector<CartItem>& items, const string& orderDate) {
		this->orderId = orderId;
		this->items = items;
		this->orderDate = orderDate;
		calculateTotal();
	}

	void setOrderId(int orderId) {
		if (orderId <= 0) {
			throw MyException("Xet: Sifaris ID-si 0-dan boyuk olmalidir!");
		}

		this->orderId = orderId;
	}


	int getOrderId() const {
		return orderId;
	}

	void setItems(const vector<CartItem>& items) {
		if (items.empty()) {
			throw MyException("Sifaris bos ola bilmez!");
		}

		this->items = items;
		calculateTotal();
	}

	const vector<CartItem>& getItems() const {
		return items;
	}

	void setOrderDate(const string& orderDate) {
		if (orderDate.empty()) {
			throw MyException("Sifaris tarixini qeyd edin!");
		}

		this->orderDate = orderDate;
	}


	string getOrderDate() const {
		return orderDate;
	}

	// umumu meblegi hesablamaq 
	void calculateTotal() {
		totalAmount = 0.0;
		for (size_t i = 0; i < items.size(); ++i) {
			totalAmount += items[i].getTotalPrice();
		}
	}

	double getTotalAmount() const {
		return totalAmount;
	}

	void showOrderInfo() const {
		cout << "----------------------------------------\n"
			<< " Sifaris      : " << orderId << "\n"
			<< " Tarix         : " << orderDate << "\n"
			<< " Umumi Mebleg  : " << totalAmount << " AZN\n"
			<< "----------------------------------------\n"
			<< " Sifaris edilen mehsullar:\n";

		for (size_t i = 0; i < items.size(); ++i) {
			cout << "  " << i + 1 << ". ";
			items[i].showInfo();
		}

		cout << "---------------------------------------" << endl;

	}

	void WriteOrdersToFile(const vector<Order>& orders, string path = "orders.txt");

	void ReadOrdersFromFile(vector<Order>& orders, string path = "orders.txt");

};

void Order::WriteOrdersToFile(const vector<Order>& orders, string path) {
	ofstream fs(path, ios::out);

	if (fs.is_open()) {

		for (const Order& o : orders) {

			fs << o.getOrderId() << "#";
			fs << o.getOrderDate() << "#";
			fs << o.getItems().size();

			for (const CartItem& item : o.getItems()) {

				const Food& f = item.getFood();

				fs << "#" << f.getId();
				fs << "#" << f.getName();
				fs << "#" << f.getDescription();
				fs << "#" << f.getPrice();
				fs << "#" << f.getCategory();
				fs << "#" << f.isAvailable();
				fs << "#" << f.getRecipe().size();

				for (const RecipeItem& r : f.getRecipe()) {
					fs << "#" << r.ingredientId;
					fs << "#" << r.requiredAmountKg;
				}

				fs << "#" << item.getQuantity();
				fs << "#" << item.getCustomNote();
				fs << "#" << item.getExtraIngredients().size();

				for (const RecipeItem& extra : item.getExtraIngredients()) {
					fs << "#" << extra.ingredientId;
					fs << "#" << extra.requiredAmountKg;
				}
			}

			fs << endl;
		}

		fs.close();
	}
	else {
		throw "File couldn't open...";
	}
}

void Order::ReadOrdersFromFile(vector<Order>& orders, string path) {
	ifstream fs(path);

	if (fs.is_open()) {

		while (!fs.eof()) {

			string row;
			getline(fs, row);

			if (row.empty()) {
				continue;
			}

			vector<string> values;
			string value = "";

			for (char character : row) {

				if (character == '#') {
					values.push_back(value);
					value = "";
				}
				else {
					value += character;
				}
			}

			values.push_back(value);

			int orderId = stoi(values[0]);
			string orderDate = values[1];
			int itemCount = stoi(values[2]);

			int counter = 3;

			vector<CartItem> items;

			for (int i = 0; i < itemCount; i++) {

				string id = values[counter++];
				string name = values[counter++];
				string description = values[counter++];
				double price = stod(values[counter++]);
				string category = values[counter++];
				bool available = stoi(values[counter++]);

				int recipeCount = stoi(values[counter++]);

				Food food(id, name, description, price, category);
				food.setAvailable(available);

				for (int j = 0; j < recipeCount; j++) {

					RecipeItem r;

					r.ingredientId = values[counter++];
					r.requiredAmountKg = stod(values[counter++]);

					food.addRecipeItem(r);
				}

				int quantity = stoi(values[counter++]);
				string customNote = values[counter++];

				CartItem cartItem(food, quantity, customNote);

				int extraCount = stoi(values[counter++]);

				for (int j = 0; j < extraCount; j++) {

					string ingredientId = values[counter++];
					double amount = stod(values[counter++]);

					cartItem.addExtraIngredient(ingredientId, amount);
				}

				items.push_back(cartItem);
			}

			Order order(orderId, items, orderDate);

			orders.push_back(order);
		}

		fs.close();
	}
	else {
		throw "File not found...";
	}
}



class Payment {
private:
	int paymentId;
	int orderId;
	double amount;
	string paymentDate;
	bool isPaid;

public:
	Payment() {
		paymentId = 0;
		orderId = 0;
		amount = 0.0;
		paymentDate = "";
		isPaid = false;
	}

	Payment(int paymentId, int orderId, double amount, string paymentDate) {
		setPaymentId(paymentId);
		setOrderId(orderId);
		setAmount(amount);
		setPaymentDate(paymentDate);
		isPaid = false;
	}

	void setPaymentId(int paymentId) {
		if (paymentId <= 0) {
			throw MyException("Odenis ID-si 0-dan boyuk olmalidir olmalidir");
		}
		this->paymentId = paymentId;
	}

	int getPaymentId() const {
		return paymentId;
	}

	void setOrderId(int orderId) {
		if (orderId <= 0) {
			throw MyException("Sifaris ID-si 0-dan boyuk olmalidir");
		}
		this->orderId = orderId;
	}

	int getOrderId() const {
		return orderId;
	}

	void setAmount(double amount) {
		if (amount <= 0) {
			throw MyException("Mebleg sifirdan boyuk olmalidir.");
		}
		this->amount = amount;
	}

	double getAmount() const {
		return amount;
	}

	void setPaymentDate(string paymentDate) {
		if (paymentDate.empty()) {
			throw MyException("Odenis tarixi bos ola bilmez!");
		}
		this->paymentDate = paymentDate;
	}

	string getPaymentDate() const {
		return paymentDate;
	}

	void setIsPaid(bool status) {
		isPaid = status;
	}

	bool getIsPaid() const {
		return isPaid;
	}

	void makePayment(const Order& order) {
		if (isPaid == true) {
			throw MyException("Bu sifarisin odenisi artiq edilib");
		}

		if (order.getOrderId() != orderId) {
			throw MyException("Sifaris ID-si uygun deyil");
		}

		if (order.getTotalAmount() <= 0) {
			throw MyException("Sifaris meblegi yanlisdir");
		}

		amount = order.getTotalAmount();
		isPaid = true;

		cout << "Odenis ugurla tamamlandi!" << endl;
		cout << "Odenis ID-si: " << paymentId << endl;
		cout << "Sifaris ID-si: " << orderId << endl;
		cout << "Odenilen mebleg: " << amount << " AZN" << endl;
	}

	void showPaymentInfo() const {
		cout << "\n========== ODENIS MELUMATLARI ==========" << endl;
		cout << "Odenis ID-si: " << paymentId << endl;
		cout << "Sifaris ID-si: " << orderId << endl;
		cout << "Mebleg: " << amount << " AZN" << endl;
		cout << "Odenis tarixi: " << paymentDate << endl;

		if (isPaid == true) {
			cout << "Status: Odenilib" << endl;
		}
		else {
			cout << "Status: Odenilmeyib" << endl;
		}

		cout << "========================================" << endl;
	}

	void WritePaymentsToFile(const vector<Payment>& payments, string path = "payments.txt") {

		ofstream fs(path, ios::out);

		if (fs.is_open()) {

			for (const Payment& p : payments) {

				fs << p.getPaymentId() << "#";
				fs << p.getOrderId() << "#";
				fs << p.getAmount() << "#";
				fs << p.getPaymentDate() << "#";
				fs << p.getIsPaid() << endl;
			}

			fs.close();
		}
		else {
			throw "File couldn't open...";
		}
	}


	void ReadPaymentsFromFile(vector<Payment>& payments, string path = "payments.txt") {

		ifstream fs(path);

		if (fs.is_open()) {

			while (!fs.eof()) {

				string row;
				getline(fs, row);

				if (row.empty()) {
					continue;
				}

				string paymentId, orderId, amount, paymentDate, isPaid;
				string value = "";
				int counter = 0;

				for (char character : row) {

					if (character == '#') {

						if (counter == 0) paymentId = value;
						else if (counter == 1) orderId = value;
						else if (counter == 2) amount = value;
						else if (counter == 3) paymentDate = value;

						value = "";
						counter++;
					}
					else {
						value += character;
					}
				}

				isPaid = value;

				Payment p(
					stoi(paymentId),
					stoi(orderId),
					stod(amount),
					paymentDate
				);

				p.setIsPaid(stoi(isPaid));

				payments.push_back(p);
			}

			fs.close();
		}
		else {
			throw "File not found...";
		}
	}

};


// =================== ADMIN PANEL =================================================










class Account {
protected:
	string id;
	string fullname;
	string email;
	string password;

public:


	Account() {
		id = "";
		email = "";
		password = "";
	}

	Account(string id, string email, string password) {
		setId(id);
		setEmail(email);
		setPassword(password);
	}

	void setId(string id) {
		if (id.size() < 3) {
			throw MyException("ID en azi 3 simvoldan ibaret olmalidir");
		}

		this->id = id;
	}

	void setFullname(string fullname) {
		if (fullname.size() < 3) {
			throw MyException("Ad ve soyad duzgun daxil edilmelidir");
		}

		this->fullname = fullname;
	}

	void setEmail(string email) {
		if (email.size() < 5 || email.find('@') == string::npos ||
			email.find('.') == string::npos) {
			throw MyException("Email unvani duzgun daxil edilmeyib");
		}

		this->email = email;
	}

	void setPassword(string password) {
		if (password.size() < 6) {
			throw MyException("Sifre en azi 6 simvoldan ibaret olmalidir");
		}

		this->password = password;
	}

	string getId() const {
		return id;
	}

	string getFullname() const {
		return fullname;
	}

	string getEmail() const {
		return email;
	}

	string getPassword() const {
		return password;
	}

	virtual void showInfo() const = 0;

	virtual ~Account() {}
};

class User : public Account {
private:
	string name;
	string surname;
	Cart cart;
	vector<Order> history;

public:
	User() {
		name = "";
		surname = "";
	}

	User(string id, string username, string password, string name, string surname)
		: Account(id, username, password) {

		setName(name);
		setSurname(surname);
	}

	void setUsername(string username) {
		setEmail(username);
	}

	void setName(string name) {
		if (name.size() < 2) {
			throw MyException("Ad duzgun daxil edilmelidir");
		}

		this->name = name;
	}

	void setSurname(string surname) {
		if (surname.size() < 2) {
			throw MyException("Soyad duzgun daxil edilmelidir");
		}

		this->surname = surname;
	}

	string getUsername() const {
		return getEmail();
	}

	string getName() const {
		return name;
	}

	string getSurname() const {
		return surname;
	}

	Cart& getCart() {
		return cart;
	}

	void addOrderToHistory(const Order& order) {
		history.push_back(order);
	}

	const vector<Order>& getHistory() const {
		return history;
	}

	void showOrderHistory() const {
		cout << "\n=== SIFARIS TARIXCESI ===" << endl;
		cout << "Ad: " << name << endl;
		cout << "Soyad: " << surname << endl;

		if (history.size() == 0) {
			cout << "Hele ki hec bir sifaris yoxdur." << endl;
			return;
		}

		for (size_t i = 0; i < history.size(); ++i) {
			history[i].showOrderInfo();
		}

		cout << "========================================" << endl;
	}

	void showInfo() const override {
		cout << "Istifadeci ID: " << id << endl;
		cout << "Istifadeci adi: " << email << endl;
		cout << "Ad: " << name << endl;
		cout << "Soyad: " << surname << endl;
	}

	void showUserInfo() const {
		showInfo();
	}

	void WriteUsersToFile(const vector<User*>& users, string path = "users.txt") {

		ofstream fs(path, ios::out);

		if (fs.is_open()) {

			for (const User* u : users) {

				fs << u->getId() << "#";
				fs << u->getUsername() << "#";
				fs << u->getPassword() << "#";
				fs << u->getName() << "#";
				fs << u->getSurname() << endl;
			}

			fs.close();
		}
		else {
			throw string("File couldn't open...");
		}
	}


	void ReadUsersFromFile(vector<User*>& users, string path = "users.txt") {

		ifstream fs(path);

		if (fs.is_open()) {

			for (User* user : users) {
				delete user;
			}
			users.clear();

			while (!fs.eof()) {

				string row;
				getline(fs, row);

				if (row.empty()) {
					continue;
				}

				string id, username, password, name, surname;
				string value = "";
				int counter = 0;

				for (char character : row) {

					if (character == '#') {

						if (counter == 0) id = value;
						else if (counter == 1) username = value;
						else if (counter == 2) password = value;
						else if (counter == 3) name = value;

						value = "";
						counter++;
					}
					else {
						value += character;
					}
				}

				surname = value;

				User* u = new User(id, username, password, name, surname);

				users.push_back(u);
			}

			fs.close();
		}
		else {
			throw string("File not found...");
		}
	}
};


bool userLogin(const User& user) {

	string username, password;

	cout << "\n===== USER GIRISI =====" << endl;

	cout << "Istifadeci adi: ";
	cin >> username;

	cout << "Sifre: ";
	cin >> password;

	if (username == user.getUsername() &&
		password == user.getPassword()) {

		cout << "Giris ugurludur!" << endl;
		return true;
	}

	cout << "Istifadeci adi ve ya sifre yanlisdir!" << endl;
	return false;
}







class Admin : public Account {

public:
	Admin() : Account() {}

	Admin(const string& id, const string& username, const string& password)
		: Account(id, username, password) {
	}

	void setUsername(const string& username) {
		setEmail(username);
	}

	string getUsername() const {
		return getEmail();
	}

	void showInfo() const override {
		cout << "Admin ID: " << id
			<< " | Istifadeci adi: " << email << endl;
	}

	// Evvelki metodun saxlanilmasi
	void showAdminInfo() const {
		showInfo();
	}

	void WriteAdminsToFile(const vector<Admin*>& admins, string path = "admins.txt") {

		ofstream fs(path, ios::out);

		if (fs.is_open()) {

			for (const Admin* a : admins) {

				fs << a->getId() << "#";
				fs << a->getUsername() << "#";
				fs << a->getPassword() << endl;
			}

			fs.close();
		}
		else {
			throw "File couldn't open...";
		}
	}


	void ReadAdminsFromFile(vector<Admin*>& admins, string path = "admins.txt") {

		ifstream fs(path);

		if (fs.is_open()) {

			while (!fs.eof()) {

				string row;
				getline(fs, row);

				if (row.empty()) {
					continue;
				}

				string id, username, password;
				string value = "";
				int counter = 0;

				for (char character : row) {

					if (character == '#') {

						if (counter == 0) id = value;
						else if (counter == 1) username = value;

						value = "";
						counter++;
					}
					else {
						value += character;
					}
				}

				password = value;

				Admin* admin = new Admin(id, username, password);

				admins.push_back(admin);
			}

			fs.close();
		}
		else {
			throw "File not found...";
		}
	}


};

bool adminLogin(const Admin& admin) {

	string username, password;

	cout << "\n===== ADMIN GIRISI =====" << endl;

	cout << "Istifadeci adi: ";
	cin >> username;

	cout << "Sifre: ";
	cin >> password;

	if (username == admin.getUsername() &&
		password == admin.getPassword()) {

		cout << "Admin girisi ugurludur!" << endl;
		return true;
	}

	cout << "Istifadeci adi ve ya sifre yanlisdir!" << endl;
	return false;


}


class Restaurant {
private:
	double budget;
	Menu menu;
	Inventory inventory; // Anbar
	vector<User*> users;
	vector<Admin*> admins;
	vector<Order> allOrders; // butun sifarisler
	vector<Payment> payments;
	int nextOrderId;

public:

	Restaurant() {
		budget = 0.0;
		nextOrderId = 1;
	}

	Restaurant(double initialBudget) {
		setBudget(initialBudget);
		nextOrderId = 1;
	}

	void setBudget(double budget) {
		if (budget < 0.0) {
			throw MyException("Restoranin budcesi menfi ola bilmez");
		}

		this->budget = budget;
	}

	double getBudget() const {
		return budget;
	}

	// Budceye pul elave etmek
	void addBudget(double amount) {
		if (amount <= 0.0) {
			throw MyException("Elave edilen mebleg 0-dan boyuk olmalidir");
		}

		budget += amount;
	}

	// Budcede pul cixarmaq
	void subtractBudget(double amount) {
		if (amount <= 0.0) {
			throw MyException("Cixilan mebleg 0-dan boyuk olmalidir");
		}

		if (budget < amount) {
			throw MyException("Restoranin budcesi kifayet qeder deyil");
		}

		budget -= amount;
	}

	Menu& getMenu() {
		return menu;
	}

	const Menu& getMenu() const {
		return menu;
	}

	Inventory& getInventory() {
		return inventory;
	}

	const Inventory& getInventory() const {
		return inventory;
	}

	// Novbeti sifaris ID-si
	int generateOrderId() {
		return nextOrderId++;
	}

	// Istifadeci qeydiyyati
	void registerUser(User* user) {

		if (user == nullptr) {
			throw MyException("User obyekti bosdur");
		}

		users.push_back(user);
	}

	// Admin elave etmek
	void addAdmin(Admin* admin) {

		if (admin == nullptr) {
			throw MyException("Admin obyekti bosdur");
		}

		admins.push_back(admin);
	}

	vector<User*>& getUsers() {
		return users;
	}

	vector<Admin*>& getAdmins() {
		return admins;
	}

	const vector<Order>& getAllOrders() const {
		return allOrders;
	}


	void addPayment(const Payment& payment) {
		payments.push_back(payment);
	}

	const vector<Payment>& getPayments() const {
		return payments;
	}

	// Yeni sifaris elave etmek
	void addOrder(const Order& order) {

		if (order.getTotalAmount() <= 0.0) {
			throw MyException("Sifarisin meblegi duzgun deyil");
		}

		addBudget(order.getTotalAmount());

		allOrders.push_back(order);
	}

	// Butun sifarisleri gostermek
	void showAllOrders() const {

		cout << "\n===== RESTORANIN BUTUN SIFARISLERI =====" << endl;

		if (allOrders.size() == 0) {
			cout << "Hele ki hec bir sifaris yoxdur" << endl;
			return;
		}

		for (size_t i = 0; i < allOrders.size(); ++i) {
			cout << "\nSifaris #" << i + 1 << endl;
			allOrders[i].showOrderInfo();
		}

		cout << "Restoranin cari budcesi: " << budget << " AZN" << endl;
		cout << "========================================" << endl;
	}

	void SaveData() {

		menu.WriteMenuToFile(menu);
		inventory.WriteToFile();

		User tempUser;
		tempUser.WriteUsersToFile(users);

		Admin tempAdmin;
		tempAdmin.WriteAdminsToFile(admins);

		Order tempCartItem;
		tempCartItem.WriteOrdersToFile(allOrders);

		Payment tempPayment;
		tempPayment.WritePaymentsToFile(payments);

		// Restoranin budcesini ve novbeti sifaris ID-sini saxlayir
		ofstream fs("restaurant.txt", ios::out);

		if (fs.is_open()) {

			fs << budget << "#" << nextOrderId << endl;

			fs.close();
		}
		else {
			throw string("Restaurant file couldn't open...");
		}

		cout << "Butun melumatlar fayllara yazildi!" << endl;
	}


	void LoadData() {

		inventory.ReadFromFile();
		menu.ReadMenuFromFile(menu);

		User tempUser;
		tempUser.ReadUsersFromFile(users);

		Admin tempAdmin;
		tempAdmin.ReadAdminsFromFile(admins);

		Order tempCartItem;
		tempCartItem.ReadOrdersFromFile(allOrders);

		Payment tempPayment;
		tempPayment.ReadPaymentsFromFile(payments);

		// Restoranin budcesini ve novbeti sifaris ID-sini oxuyur
		ifstream fs("restaurant.txt");

		if (fs.is_open()) {

			string row;
			getline(fs, row);

			string str_budget = "";
			string str_nextOrderId = "";
			int counter = 0;

			for (char character : row) {

				if (character == '#') {
					counter++;
				}
				else if (counter == 0) {
					str_budget += character;
				}
				else if (counter == 1) {
					str_nextOrderId += character;
				}
			}

			if (str_budget.size() != 0 && str_nextOrderId.size() != 0) {

				budget = stod(str_budget);
				nextOrderId = stoi(str_nextOrderId);
			}

			fs.close();
		}
		else {
			throw string("Restaurant file not found...");
		}

		cout << "Butun melumatlar fayllardan oxundu!" << endl;
	}


	~Restaurant() {

		for (User* user : users) {
			delete user;
		}

		for (Admin* admin : admins) {
			delete admin;
		}
	}
};


void completeOrder(User& user, Restaurant& restaurant) {

	Cart& cart = user.getCart();

	if (cart.isEmpty()) {
		throw MyException("Sebet bosdur!");
	}

	vector<CartItem> items = cart.getItems();

	// Evvelce butun mehsullar ucun stok yoxlanilir
	for (const CartItem& item : items) {

		for (const RecipeItem& recipe : item.getFood().getRecipe()) {

			double requiredAmount =
				recipe.requiredAmountKg * item.getQuantity();

			if (!restaurant.getInventory().hasEnoughStock(
				recipe.ingredientId, requiredAmount)) {

				throw MyException("Anbarda kifayet qeder inqrediyent yoxdur!");
			}
		}
	}

	string orderDate;

	cout << "Sifaris tarixi: ";
	cin >> orderDate;

	int orderId = restaurant.generateOrderId();

	Order order(orderId, items, orderDate);

	order.calculateTotal();

	cout << "\nOdenilecek mebleg: "
		<< order.getTotalAmount() << " AZN" << endl;

	Payment payment(orderId, orderId, order.getTotalAmount(), orderDate);

	payment.makePayment(order);

	restaurant.addPayment(payment);

	// Sifaris ugurla odenildikden sonra stokdan cixilir
	for (const CartItem& item : items) {

		for (int i = 0; i < item.getQuantity(); i++) {
			restaurant.getInventory().consumeRecipe(item.getFood());
		}
	}

	restaurant.getMenu().updateAvailability(
		restaurant.getInventory()
	);

	restaurant.addOrder(order);

	user.addOrderToHistory(order);

	cart.clearCart();

	cout << "\nSifaris ugurla tamamlandi!" << endl;
	payment.showPaymentInfo();
	order.showOrderInfo();
}



void showAdminPanel(Restaurant& restaurant) {

	int choice;

	do {
		cout << "\n========== ADMIN PANEL ==========" << endl;
		cout << "1. Inqrediyent elave et" << endl;
		cout << "2. Anbari goster" << endl;
		cout << "3. Stoku artir" << endl;
		cout << "4. Stoku azalt" << endl;
		cout << "5. Menyunu goster" << endl;
		cout << "6. Butun sifarisleri goster" << endl;
		cout << "7. Yemek elave et" << endl;
		cout << "8. Yemek sil" << endl;
		cout << "0. Cixis" << endl;

		cout << "Seciminiz: ";
		cin >> choice;

		try {

			switch (choice) {

			case 1: {
				string id, name;
				double price, quantity;

				cout << "Inqrediyentin ID-si: ";
				cin >> id;

				cout << "Inqrediyentin adi: ";
				cin >> name;

				cout << "1 kq ucun qiymet: ";
				cin >> price;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					throw MyException("Qiymet reqem olmalidir!");
				}
				cout << "Ilkin miqdar (kg): ";
				cin >> quantity;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					throw MyException("Miqdar reqem olmalidir!");
				}

				Ingredient ingredient(id, name, price);

				restaurant.getInventory().addIngredient(ingredient, quantity);

				restaurant.subtractBudget(price * quantity);
				restaurant.SaveData();

				cout << "1 kq ucun qiymet: ";
				cin >> price;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					throw MyException("Qiymet reqem olmalidir!");
				}

				cout << "Inqrediyent elave edildi!" << endl;
				break;
			}

			case 2: {
				restaurant.getInventory().showInventory();
				break;
			}

			case 3: {
				string id;
				double quantity;

				cout << "Inqrediyentin ID-si: ";
				cin >> id;

				cout << "Artirilacaq miqdar: ";
				cin >> quantity;

				restaurant.getInventory().increaseStock(id, quantity);

				cout << "Stok artirildi!" << endl;
				break;
			}

			case 4: {
				string id;
				double quantity;

				cout << "Inqrediyentin ID-si: ";
				cin >> id;

				cout << "Azaldilacaq miqdar: ";
				cin >> quantity;

				restaurant.getInventory().decreaseStock(id, quantity);

				cout << "Stok azaldildi!" << endl;
				break;
			}

			case 5: {
				restaurant.getMenu().showMenu();
				break;
			}

			case 6: {
				restaurant.showAllOrders();
				break;
			}

			case 7: {
				string id, name, description, category;
				double price;
				int recipeCount;

				cout << "Yemeyin ID-si: ";
				cin >> id;

				cout << "Yemeyin adi: ";
				cin >> name;

				cout << "Tesviri: ";
				cin >> description;

				cout << "Qiymeti: ";
				cin >> price;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');

					throw MyException("Qiymet reqem olmalidir!");
				}

				cout << "Kateqoriyasi: ";
				cin >> category;

				cout << "Nece inqrediyent istifade olunur? ";
				cin >> recipeCount;

				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');

					throw MyException(
						"Inqrediyent sayi reqem olmalidir!"
					);
				}

				if (recipeCount <= 0) {
					throw MyException(
						"Reseptde en azi bir inqrediyent olmalidir"
					);
				}
				Food food(id, name, description, price, category);

				for (int i = 0; i < recipeCount; i++) {

					string ingredientId;
					double amount;

					cout << "Inqrediyentin ID-si: ";
					cin >> ingredientId;

					if (cin.fail()) {
						cin.clear();
						cin.ignore(10000, '\n');

						throw MyException("Inqrediyent ID-si duzgun daxil edilmeyib!");
					}

					if (restaurant.getInventory().containsIngredient(ingredientId) == false(
						ingredientId)) {
						throw MyException(
							"Bu inqrediyent anbarda movcud deyil"
						);
					}
					cout << "Teleb olunan miqdar (kg): ";
					cin >> amount;

					if (cin.fail()) {
						cin.clear();
						cin.ignore(10000, '\n');

						throw MyException("Miqdar reqem olmalidir!");
					}

					RecipeItem item;
					item.ingredientId = ingredientId;
					item.requiredAmountKg = amount;

					food.addRecipeItem(item);
				}

				restaurant.getMenu().addFood(food);

				restaurant.getMenu().updateAvailability(
					restaurant.getInventory()
				);

				restaurant.SaveData();

				cout << "Yemek menyuya elave edildi!" << endl;
				break;
			}

			case 8: {
				string id;

				cout << "Silinecek yemeyin ID-si: ";
				cin >> id;

				restaurant.getMenu().removeFood(id);

				cout << "Yemek silindi!" << endl;
				break;
			}

			case 0:
				cout << "Admin panelden cixildi!" << endl;
				break;

			default:
				cout << "Yanlis secim etdiniz!" << endl;
				break;
			}
		}

		catch (MyException& ex) {
			ex.what();
		}

		catch (string& error) {
			cout << "Xeta: " << error << endl;
		}

	} while (choice != 0);
}


void showUserPanel(User& user, Restaurant& restaurant) {

	int choice;

	do {
		cout << "\n========== USER PANEL ==========" << endl;
		cout << "1. Menyunu goster" << endl;
		cout << "2. Sebete yemek elave et" << endl;
		cout << "3. Sebetden yemek sil" << endl;
		cout << "4. Yemek sayini deyis" << endl;
		cout << "5. Sebete bax" << endl;
		cout << "6. Sifarisi tesdiq et ve odenis et" << endl;
		cout << "7. Sifaris tarixcesi" << endl;
		cout << "0. Panelden cix" << endl;

		cout << "Seciminiz: ";
		cin >> choice;

		try {

			switch (choice) {

			case 1: {
				restaurant.getMenu().showMenu();
				break;
			}

			case 2: {
				string id, note;
				int quantity;

				cout << "Yemeyin ID-si: ";
				cin >> id;

				cout << "Miqdar: ";
				cin >> quantity;

				cout << "Elave qeyd: ";
				cin.ignore();
				getline(cin, note);

				Food& food = restaurant.getMenu().findFoodById(id);

				user.getCart().addItem(food, quantity, note);

				cout << "Yemek sebete elave edildi!" << endl;
				break;
			}

			case 3: {
				string id;

				cout << "Silinecek yemeyin ID-si: ";
				cin >> id;

				user.getCart().removeItem(id);

				cout << "Yemek sebetden silindi!" << endl;
				break;
			}

			case 4: {
				string id;
				int quantity;

				cout << "Yemeyin ID-si: ";
				cin >> id;

				cout << "Yeni miqdar: ";
				cin >> quantity;

				user.getCart().updateQuantity(id, quantity);

				cout << "Miqdar deyisdirildi!" << endl;
				break;
			}

			case 5: {
				user.getCart().showCart();
				break;
			}

			case 6: {
				completeOrder(user, restaurant);
				break;
			}

			case 7: {
				user.showOrderHistory();
				break;
			}

			case 0:
				cout << "User panelden cixildi!" << endl;
				break;

			default:
				cout << "Yanlis secim etdiniz!" << endl;
				break;
			}
		}

		catch (MyException& ex) {
			ex.what();
		}

		catch (string& error) {
			cout << "Xeta: " << error << endl;
		}

	} while (choice != 0);
}


void main() {

	system("color 0B");

	Restaurant restaurant(1000);

	// Fayllardaki melumatlari oxuyuruq
	bool dataLoaded = true;

	try {
		restaurant.LoadData();
	}
	catch (MyException& ex) {
		ex.what();
		dataLoaded = false;
	}
	catch (const string& error) {
		cout << "\nXeta: " << error << endl;
		dataLoaded = false;
	}
	catch (const char* error) {
		cout << "\nXeta: " << error << endl;
		dataLoaded = false;
	}

	if (dataLoaded == false) {
		cout << "\nMelumatlar yuklenmedi. Proqram dayandirilir!\n";
	}

	
	if (restaurant.getAdmins().size() == 0) {

		Admin* admin = new Admin("A01", "admin@gmail.com", "admin123");

		restaurant.addAdmin(admin);
		restaurant.SaveData();

		cout << "\nIlkin admin ugurla yaradildi!\n";
	}

	int choice = -1;

	while (true) {

		try {

			cout << "\n";
			cout << "========================================\n";
			cout << "           RESTORAN SISTEMI             \n";
			cout << "========================================\n";
			cout << "  1. Admin girisi                      \n";
			cout << "  2. Istifadeci qeydiyyati             \n";
			cout << "  3. Istifadeci girisi                 \n";
			cout << "  0. Proqramdan cixis                  \n";
			cout << "========================================\n";
			cout << "  Seciminizi daxil edin: ";

			cin >> choice;

			if (cin.fail()) {

				if (cin.eof()) {
					cout << "\nInput baglandi. Proqramdan cixilir.\n";
					
				}

				cin.clear();
				cin.ignore(10000, '\n');

				cout << "\nSecim reqem olmalidir!\n";
				continue;
			}

			if (choice == 1) {

				string email, password;

				cout << "\n---------- ADMIN GIRISI ----------\n";

				cout << "Email: ";
				cin >> email;

				cout << "Sifre: ";
				cin >> password;

				bool found = false;

				for (Admin* admin : restaurant.getAdmins()) {

					if (admin->getEmail() == email &&
						admin->getPassword() == password) {

						found = true;

						cout << "\nAdmin girisi ugurludur!\n";

						showAdminPanel(restaurant);

						break;
					}
				}

				if (found == false) {
					cout << "\nEmail ve ya sifre yanlisdir!\n";
				}
			}

			else if (choice == 2) {

				string id, email, password, name, surname;

				cout << "\n------- ISTIFADECI QEYDIYYATI -------\n";

				cout << "ID: ";
				cin >> id;

				cout << "Email: ";
				cin >> email;

				cout << "Sifre: ";
				cin >> password;

				cout << "Ad: ";
				cin >> name;

				cout << "Soyad: ";
				cin >> surname;

				User* user = new User(id, email, password, name, surname);

				restaurant.registerUser(user);

				restaurant.SaveData();

				cout << "\nQeydiyyat ugurla tamamlandi!\n";
			}

			else if (choice == 3) {

				string email, password;

				cout << "\n--------- ISTIFADECI GIRISI ---------\n";

				cout << "Email: ";
				cin >> email;

				cout << "Sifre: ";
				cin >> password;

				bool found = false;

				for (User* user : restaurant.getUsers()) {

					if (user->getUsername() == email &&
						user->getPassword() == password) {

						found = true;

						cout << "\nSisteme ugurla daxil oldunuz!\n";

						showUserPanel(*user, restaurant);

						break;
					}
				}

				if (found == false) {
					cout << "\nEmail ve ya sifre yanlisdir!\n";
				}
			}

			else if (choice == 0) {

				restaurant.SaveData();

				cout << "\n========================================\n";
				cout << "       Proqramdan cixis edilir...\n";
				cout << "       Melumatlar yadda saxlanildi!\n";
				cout << "========================================\n";

				break;
			}

			else {

				cout << "\nYanlis secim etdiniz! Yeniden cehd edin.\n";
			}
		}

		catch (MyException& ex) {
			ex.what();
		}
		catch (const string& error) {
			cout << "\nXeta: " << error << endl;
		}
		catch (const char* error) {
			cout << "\nXeta: " << error << endl;
		}
	}

	
}


