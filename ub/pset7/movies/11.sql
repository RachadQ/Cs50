select title from movies inner join stars on movies.id = stars.movie_id
inner join people on people.id = stars.person_id
inner join ratings on ratings.movie_id = movies.id where people.name = "Chadwick Boseman" order by rating  desc limit 5