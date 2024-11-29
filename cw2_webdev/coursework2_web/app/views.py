from app import app,db,models,admin,forms
from flask import render_template, flash, redirect, url_for
from flask_admin.contrib.sqla import ModelView
from werkzeug.security import generate_password_hash

admin.add_view(ModelView(models.User, db.session))
admin.add_view(ModelView(models.Recipe, db.session))

@app.route('/', endpoint='index')
@app.route('/all', endpoint='all')
def index():
    return render_template('all.html', title='Recipe App', active_page='all')

@app.route('/login',endpoint='login',methods=['GET', 'POST'])
def login():
    form = forms.LoginForm()
    if form.validate_on_submit():
        password = form.password.data
        username= form.username.data
        print(f"Debug: Username entered: {username}")
        print(f"Debug: Password entered: {password}")
        if forms.validate_user_credentials(username,password):
            flash('succesfully registered in')
            return redirect(url_for('all'))
    return render_template('login.html',title='login',active_page='login',form=form)

@app.route('/register',endpoint='register',methods=['GET', 'POST'])
def register():
    form = forms.LoginForm()
    if form.validate_on_submit():
        password = form.password.data
        username= form.username.data
        print(f"Debug: Username entered: {username}")
        print(f"Debug: Password entered: {password}")
        if forms.validate_user(username):
            print("succesfully verified")
            hashed_password = generate_password_hash(password)
            new_user = models.User(username=username,hashed_password=hashed_password)
            db.session.add(new_user)
            db.session.commit()
            flash('succesfully registered in')
            return redirect(url_for('all'))
        else:
            print("something wrong")
        print("hello world")
    return render_template('register.html',title='register',active_page='register',form=form)

@app.route('/appetizers', endpoint='appetizers')
def appetizers():
    return render_template('appetizers.html', title='Recipe App', active_page='appetizers')

@app.route('/beverages', endpoint='beverages')
def beverages():
    return render_template('beverages.html', title='Recipe App', active_page='beverages')

@app.route('/breakfast', endpoint='breakfast')
def breakfast():
    return render_template('breakfast.html', title='Recipe App', active_page='breakfast')

@app.route('/dessert', endpoint='dessert')
def dessert():
    return render_template('dessert.html', title='Recipe App', active_page='dessert')

@app.route('/main_course', endpoint='main_course')
def main_course():
    return render_template('main_course.html', title='Recipe App', active_page='main_course')

@app.route('/salad', endpoint='salad')
def salad():
    return render_template('salad.html', title='Recipe App', active_page='salad')

@app.route('/sauces', endpoint='sauces')
def sauces():
    return render_template('sauces.html', title='Recipe App', active_page='sauces')

@app.route('/side_dish', endpoint='side_dish')
def side_dish():
    return render_template('side_dish.html', title='Recipe App', active_page='side_dish')

@app.route('/snacks', endpoint='snacks')
def snacks():
    return render_template('snacks.html', title='Recipe App', active_page='snacks')

@app.route('/soups', endpoint='soups')
def soups():
    return render_template('soups.html', title='Recipe App', active_page='soups')