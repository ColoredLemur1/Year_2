from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, SubmitField, TextAreaField, SelectField
from wtforms.validators import DataRequired, Length
from app import db
from app.models import User
from werkzeug.security import check_password_hash


class LoginForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired(), Length(min=4, max=25)])
    password = PasswordField('Password', validators=[DataRequired(), Length(min=6, max=25)])
    submit = SubmitField('Login')

class RecipeForm(FlaskForm):
    title = StringField('Title', validators=[DataRequired(), Length(min=1, max=100)])
    ingredients = TextAreaField('Ingredients', validators=[DataRequired()])
    steps = TextAreaField('Steps', validators=[DataRequired()])
    category = SelectField('Category', choices=[('appetizers','Appetizers'),
    ('salad','Salad'),('soups and Stews','Soups and Stews'),
    ('main course','Main Course'),('side dish','Side Dish'),('dessert','Dessert'),('beverages','Beverages'),
    ('breakfast','Breakfast'),('snacks','Snacks'),('sauces and condiments','Sauces and Condiments')])
    submit = SubmitField('submit')

def validate_user(username):
    print("Debug: validate_user function called")
    if db.session.query(User).count() == 0:
        print("Debug: User table is empty")
        return True
    user = db.session.query(User).filter_by(username=username).first()
    if user:
        print("Debug: Username already exists")
        return False
    print("Debug: Username is available")
    return True

def validate_user_credentials(username, password):
    user = User.query.filter_by(username=username).first()
    if user and check_password_hash(user.hashed_password, password):
        return True
    return False