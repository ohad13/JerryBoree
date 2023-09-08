import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import useFetch from './useFetch';
import { useParams } from 'react-router-dom/cjs/react-router-dom.min';

const AuthorPage = () => {
    const { author } = useParams();
    const { data: blogs, isPending, error } = useFetch('http://localhost:8000/blogs');
    const [authorBlogs, setAuthorBlogs] = useState([]);
    const languages = [
        { value: 'en', label: 'English' },
        { value: 'es', label: 'Spanish' },
        { value: 'fr', label: 'French' },
        { value: 'de', label: 'German' },
        { value: 'it', label: 'Italian' },
        { value: 'pt', label: 'Portuguese' },
        { value: 'zh', label: 'Chinese' },
        { value: 'ja', label: 'Japanese' },
        { value: 'ko', label: 'Korean' },
        { value: 'ru', label: 'Russian' },
        { value: 'ar', label: 'Arabic' },
    ];
    useEffect(() => {
        if (blogs) {
            // Filter the blogs based on the author's name
            const filteredBlogs = blogs.filter((blog) => blog.author.toLowerCase() === author.toLowerCase());
            setAuthorBlogs(filteredBlogs);
        }
    }, [blogs, author]);

    return (
        <div>
            <h2 style={{ marginBottom: '20px' }}>Author: {author} </h2>
            <ul >
                {authorBlogs.map(blog => (
                    <Link to={`/blogs/${blog.id}`} key={blog.id} style={{ textDecoration: "none" }}>
                        <li className="blog-preview" >
                            <h3>Title: {blog.title}</h3>
                            <p>Likes: {blog.like}</p>
                            <p>Language: {languages.find(lang => lang.value === blog.language)?.label}</p>
                        </li>
                    </Link>
                ))}
            </ul>
        </div >
    )

};

export default AuthorPage;
