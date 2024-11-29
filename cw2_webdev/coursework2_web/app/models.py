from app import db

from datetime import datetime

likes = db.Table(
    'likes',
    db.Column('user_id', db.Integer, db.ForeignKey('user.id'), primary_key=True),
    db.Column('recipe_id', db.Integer, db.ForeignKey('recipe.id'), primary_key=True)
)

class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(100), nullable=False, unique=True)
    hashed_password = db.Column(db.String(128), nullable=False)
    liked_recipes = db.relationship(
        'Recipe',
        secondary=likes, #adds it to the association table
        backref=db.backref('liked_by', lazy='dynamic'),
        lazy='dynamic'
    )
    recipes = db.relationship('Recipe',backref='author',lazy=True)
    def __repr__(self):
        return f"<User(username ={self.username})>"

class Recipe(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(200), nullable=False)
    ingredients = db.Column(db.Text, nullable=False)
    steps = db.Column(db.Text, nullable=False)
    date = db.Column(db.DateTime, default=datetime.utcnow)    
    category = db.Column(db.String(100))
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)

    def __repr__(self):
        return f"<Recipe(title={self.title}, category={self.category}, likes={self.likes})>"
