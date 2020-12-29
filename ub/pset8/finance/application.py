import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    #get usser thats loggged in
    rows = db.execute("SELECT symbol,sum(shares) as total FROM transactions WHERE user_id = :user_id Group by symbol HAVING TOTAL > 0",user_id=session["user_id"])

    totalStock = {}
    grandTotal = 0
    #loop through all rows
    for row in rows:
        stock = lookup(row["symbol"])
        totalStock = {"symbol": stock["symbol"],"name": stock["name"],"shares": row["total"],"price": usd(stock["price"]), "total": usd(stock["price"]*row["total"])}

    rows = db.execute("SELECT cash from users WHERE id=:user_id", user_id=session["user_id"])
    cash = rows[0]["cash"]

    grandTotal += cash
    return render_template("index.html", holdings=totalStock, cash=usd(cash),grandTotal= usd(grandTotal))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        sharesAmount = request.form.get("shares")
        if not sharesAmount.isdigit():
            return apology("invalid numbers of shares")
        #look for  stock
        stock = lookup(request.form.get("symbol").upper())
        if stock is None:
            return apology("invalid symbol")
        #get users cash from database
        rows = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        loot = rows[0]["cash"]
        update = loot - (float(sharesAmount) * stock['price'])
        #check if user have valid funds
        if update < 0:
            return apology("insufficient funds",403)
            #update cash
        db.execute("Update users SET cash=:update Where id=:id",update=update,id=session["user_id"])
        #add users transaction
        db.execute("INSERT INTO transactions(user_id,symbol,shares,price) VALUES (:user_id, :symbol,:sharesAmount, :price)",user_id=session["user_id"], symbol = stock["symbol"],sharesAmount = sharesAmount,price=stock["price"])

        return redirect("/")
    else:
        return render_template("buy.html")

    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Missing symbol", 403)

        quote = lookup(request.form.get("symbol").upper())
        if quote is None:
            return apology("Can't find symbol", 403)
            # TODO: write code...
        return render_template("quoted.html",quoted={'name': quote['name'],'symbol': quote['symbol'],'price':usd(quote['price'])} )
    else:
        return render_template("quote.html")
    return redirect("/quote")



@app.route("/register", methods=["GET", "POST"])
def register():
    user = request.form.get("username")
    password = request.form.get("password")
    password2 = request.form.get("password2")

     # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
         # Ensure username was submitted
        if not user:
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 403)
        elif not password2:
             return apology("must provide matching password", 403)
        #Ensure the Password and check password match
        elif password != password2:
                 return apology("passwords don't match", 403)
        db.execute("INSERT INTO users(username,hash) VALUES(:user,:password)",user=user, password=generate_password_hash(password))
        return redirect("/")

    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
