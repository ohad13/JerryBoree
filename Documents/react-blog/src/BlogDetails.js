import { useParams } from 'react-router-dom'
import React, { useState, useEffect } from 'react';
import { useHistory } from "react-router-dom";
import useFetch from './useFetch';
import './index.css';
import { Link } from 'react-router-dom'
import Comment from './Comment';
import { v4 as uuidv4 } from 'uuid';
import PasswordConfirmation from './PasswordConfirmation';

const BlogDetails = () => {
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
    const { id } = useParams();
    const [isConfirmationOpen, setConfirmationOpen] = useState(false);
    const { data: blog, error, isPanding } = useFetch('http://localhost:8000/blogs/' + id)
    const history = useHistory();
    const [isLikeDisabled, setIsLikeDisabled] = useState(false);
    const [updatedLikeCount, setUpdatedLikeCount] = useState(null);
    const [comments, setComments] = useState([]);

    useEffect(() => {
        if (blog) {
            setUpdatedLikeCount(blog.like); // Initialize the like count
            setComments(blog.comments || []); // Initialize comments from the blog data or an empty array
        }
    }, [blog]); // Add a dependency to update the like count + comments when 'blog' changes

    const handleAddComment = (commentText) => {
        fetch(`http://localhost:8000/blogs/${id}`)
            .then((response) => response.json())
            .then((latestBlog) => {
                // Generate a unique comment ID using uuidv4
                const commentId = uuidv4();

                // Create a new comment object with a unique ID
                // const newComment = { id: commentId, text: commentText };

                // Create a copy of the existing comments array and add the new comment
                const updatedComments = [...comments, commentText]; // [..,newComment];
                setComments(updatedComments);

                // Update the blog object to include the new comments
                const updatedBlog = { ...latestBlog, comments: updatedComments };

                // Make a PUT request to update the blog with the new comments
                fetch(`http://localhost:8000/blogs/${id}`, {
                    method: 'PUT',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify(updatedBlog),
                })
                    .then(() => {
                        console.log('Comment added and blog updated!');
                    })
                    .catch((error) => {
                        console.error('Error updating the blog:', error);
                    });
            })
            .catch((error) => {
                console.error('Error fetching the latest blog:', error);
            });
    };

    const handleClickDelete = () => {
        fetch('http://localhost:8000/blogs/' + id, {
            method: "DELETE"
        }).then(() => {
            history.push('/')
        })
    }
    const handleClickHome = () => {
        history.push('/')
    }
    const handleClickLike = () => {
        if (blog && !isLikeDisabled) {
            fetch(`http://localhost:8000/blogs/${id}`)
                .then((response) => response.json())
                .then((latestBlog) => {
                    const newLikeCount = latestBlog.like !== null ? latestBlog.like + 1 : blog.like;
                    setUpdatedLikeCount(newLikeCount);

                    const updatedBlog = { ...latestBlog, like: newLikeCount };
                    const blogId = id;

                    fetch(`http://localhost:8000/blogs/${blogId}`, {
                        method: 'PUT',
                        headers: {
                            "Content-Type": "application/json"
                        },
                        body: JSON.stringify(updatedBlog)
                    }).then(() => {
                        console.log("Blog updated!");
                        setIsLikeDisabled(true);
                    });
                })
                .catch((error) => {
                    console.error('Error fetching the latest blog:', error);
                });
        }
    };
    const handleDelete = () => {
        // Show the password confirmation popup
        setConfirmationOpen(true);
    };
    const handleCancel = () => {
        // Close the password confirmation popup
        setConfirmationOpen(false);
    };
    const handleConfirmDelete = () => {
        // Perform the actual delete action here
        console.log('Blog deleted!');
        // Close the password confirmation popup
        setConfirmationOpen(false);
        handleClickDelete();
    };

    return (
        <div className="blog-details">
            {isPanding && <div>Loading...</div>}
            {error && <div>{error}</div>}
            {blog && (
                <article style={{ display: 'flex', flexDirection: 'column', alignItems: 'center' }}>
                    <div style={{ display: 'flex', alignItems: 'center', marginBottom: '20px' }}>
                        <div style={{ marginRight: '20px' }}>
                            <h2>{blog.title}</h2>
                            <p>Written by:
                                <Link style={{ marginLeft: '5px' }} to={`/authors/${blog.author}`}>{blog.author}</Link>
                            </p>
                            <p>Language: {languages.find(lang => lang.value === blog.language)?.label}</p>
                            <p>Likes: {updatedLikeCount !== null ? updatedLikeCount : blog.like}</p>
                        </div>
                        {blog.image && <img className="thumbnail-image" src={process.env.PUBLIC_URL + '/' + blog.image} alt="Blog Image" style={{ width: '200px', height: 'auto' }} />}
                    </div>
                    <div style={{ maxWidth: '600px' }}>
                        {blog.body}
                    </div>
                    <div style={{ marginTop: '20px' }}>
                        <button onClick={handleClickLike} style={{ marginRight: '10px' }} disabled={isLikeDisabled} className={isLikeDisabled ? 'disabled-button' : ''}>Like!</button>
                        <button onClick={handleDelete} style={{ marginRight: '10px' }}>Delete Blog</button>
                        <PasswordConfirmation
                            isOpen={isConfirmationOpen}
                            onRequestClose={handleCancel}
                            onConfirm={handleConfirmDelete}
                        />
                        <button onClick={handleClickHome}>Back Home</button>
                    </div>
                    <Comment comments={blog.comments} blog={blog} onAddComment={handleAddComment} />
                </article>
            )}
        </div>
    );
}

export default BlogDetails;